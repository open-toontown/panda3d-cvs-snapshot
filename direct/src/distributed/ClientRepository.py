"""ClientRepository module: contains the ClientRepository class"""

from pandac.PandaModules import *
from MsgTypes import *
from direct.task import Task
from direct.directnotify import DirectNotifyGlobal
import CRCache
import ConnectionRepository
from direct.showbase import PythonUtil
import ParentMgr
import RelatedObjectMgr
import time
from ClockDelta import *
from PyDatagram import PyDatagram
from PyDatagramIterator import PyDatagramIterator

class ClientRepository(ConnectionRepository.ConnectionRepository):
    notify = DirectNotifyGlobal.directNotify.newCategory("ClientRepository")

    def __init__(self):
        ConnectionRepository.ConnectionRepository.__init__(self, base.config)
        self.setClientDatagram(1)

        self.recorder = base.recorder
        
        self.doId2do={}
        self.readDCFile()
        self.cache=CRCache.CRCache()
        self.serverDelta = 0

        self.bootedIndex = None
        self.bootedText = None

        self.worldScale = render.attachNewNode("worldScale") # for grid zones.
        self.worldScale.setScale(base.config.GetFloat('world-scale', 100))
        self.priorWorldPos = None
        
        # create a parentMgr to handle distributed reparents
        # this used to be 'token2nodePath'
        self.parentMgr = ParentMgr.ParentMgr()

        # The RelatedObjectMgr helps distributed objects find each
        # other.
        self.relatedObjectMgr = RelatedObjectMgr.RelatedObjectMgr(self)

        # Keep track of how recently we last sent a heartbeat message.
        # We want to keep these coming at heartbeatInterval seconds.
        self.heartbeatInterval = base.config.GetDouble('heartbeat-interval', 10)
        self.heartbeatStarted = 0
        self.lastHeartbeat = 0
        if wantOtpServer:
            ##
            ## Top level Interest Manager
            ##
            self.__interest_id_assign = 1
            self.__interesthash = {}
        
    
    def abruptCleanup(self):
        """
        Call this method to clean up any pending hooks or tasks on
        distributed objects, but leave the ClientRepository in a sane
        state for creating more distributed objects.
        """
        self.relatedObjectMgr.abortAllRequests()

    def sendDisconnect(self):
        if self.isConnected():
            # Tell the game server that we're going:
            datagram = PyDatagram()
            # Add message type
            datagram.addUint16(CLIENT_DISCONNECT)
            # Send the message
            self.send(datagram)
            self.notify.info("Sent disconnect message to server")
            self.disconnect()
        self.stopHeartbeat()

    def setWorldOffset(self, xOffset=0, yOffset=0):
        self.worldXOffset=xOffset
        self.worldYOffset=yOffset

    def getWorldPos(self, nodePath):
        pos = nodePath.getPos(self.worldScale)
        return (int(round(pos.getX())), int(round(pos.getY())))

    def sendWorldPos(self, x, y):
        # The server will need to know the world
        # offset of our current render node path
        # and adjust the x, y accordingly.  At one
        # point I considered adding the world offset
        # here, but that would just use extra bits.

        onScreenDebug.add("worldPos", "%-4d, %-4d"%(x, y))
        return #*#

        datagram = PyDatagram()
        # Add message type
        datagram.addUint16(CLIENT_SET_WORLD_POS)
        # Add x
        datagram.addInt16(x)
        # Add y
        datagram.addSint16(y)
        # send the message
        self.send(datagram)

    def checkWorldPos(self, nodePath):
        worldPos = self.getWorldPos(nodePath)
        if self.priorWorldPos != worldPos:
            self.priorWorldPos = worldPos
            self.sendWorldPos(worldPos[0], worldPos[1])

    def setServerDelta(self, delta):
        """
        Indicates the approximate difference in seconds between the
        client's clock and the server's clock, in universal time (not
        including timezone shifts).  This is mainly useful for
        reporting synchronization information to the logs; don't
        depend on it for any precise timing requirements.

        Also see Notify.setServerDelta(), which also accounts for a
        timezone shift.
        """
        self.serverDelta = delta

    def getServerDelta(self):
        return self.serverDelta

    def getServerTimeOfDay(self):
        """
        Returns the current time of day (seconds elapsed since the
        1972 epoch) according to the server's clock.  This is in GMT,
        and hence is irrespective of timezones.

        The value is computed based on the client's clock and the
        known delta from the server's clock, which is not terribly
        precisely measured and may drift slightly after startup, but
        it should be accurate plus or minus a couple of seconds.
        """
        return time.time() + self.serverDelta

    def handleGenerateWithRequired(self, di):
        # Get the class Id
        classId = di.getUint16();
        # Get the DO Id
        doId = di.getUint32()
        # Look up the dclass
        dclass = self.dclassesByNumber[classId]
        dclass.startGenerate()
        # Create a new distributed object, and put it in the dictionary
        distObj = self.generateWithRequiredFields(dclass, doId, di)
        dclass.stopGenerate()

    def handleGenerateWithRequiredOther(self, di):
        # Get the class Id
        classId = di.getUint16();
        # Get the DO Id
        doId = di.getUint32()
        # Look up the dclass
        dclass = self.dclassesByNumber[classId]
        dclass.startGenerate()
        # Create a new distributed object, and put it in the dictionary
        distObj = self.generateWithRequiredOtherFields(dclass, doId, di)
        dclass.stopGenerate()

    def handleQuietZoneGenerateWithRequired(self, di):
        # Special handler for quiet zone generates -- we need to filter
        # Get the class Id
        classId = di.getUint16();
        # Get the DO Id
        doId = di.getUint32()
        # Look up the dclass
        dclass = self.dclassesByNumber[classId]
        dclass.startGenerate()
        # If the class is a neverDisable class (which implies uberzone) we
        # should go ahead and generate it even though we are in the quiet zone
        if not wantOtpServer:
            if dclass.getClassDef().neverDisable:
                # Create a new distributed object, and put it in the dictionary
                distObj = self.generateWithRequiredFields(dclass, doId, di)
        else:
            distObj = self.generateWithRequiredFields(dclass, doId, di)
        dclass.stopGenerate()

    def handleQuietZoneGenerateWithRequiredOther(self, di):
        # Special handler for quiet zone generates -- we need to filter
        # Get the class Id
        classId = di.getUint16();
        # Get the DO Id
        doId = di.getUint32()
        # Look up the dclass
        dclass = self.dclassesByNumber[classId]
        # If the class is a neverDisable class (which implies uberzone) we
        # should go ahead and generate it even though we are in the quiet zone
        dclass.startGenerate()
        if not wantOtpServer:
            if dclass.getClassDef().neverDisable:
                # Create a new distributed object, and put it in the dictionary
                distObj = self.generateWithRequiredOtherFields(dclass, doId, di)
        else:
            distObj = self.generateWithRequiredOtherFields(dclass, doId, di)
        dclass.stopGenerate()

    def generateWithRequiredFields(self, dclass, doId, di):
        if self.doId2do.has_key(doId):
            # ...it is in our dictionary.
            # Just update it.
            distObj = self.doId2do[doId]
            assert(distObj.dclass == dclass)
            distObj.generate()
            distObj.updateRequiredFields(dclass, di)
            # updateRequiredFields calls announceGenerate
        elif self.cache.contains(doId):
            # ...it is in the cache.
            # Pull it out of the cache:
            distObj = self.cache.retrieve(doId)
            assert(distObj.dclass == dclass)
            # put it in the dictionary:
            self.doId2do[doId] = distObj
            # and update it.
            distObj.generate()
            distObj.updateRequiredFields(dclass, di)
            # updateRequiredFields calls announceGenerate
        else:
            # ...it is not in the dictionary or the cache.
            # Construct a new one
            classDef = dclass.getClassDef()
            if classDef == None:
                self.notify.error("Could not create an undefined %s object." % (dclass.getName()))
            distObj = classDef(self)
            distObj.dclass = dclass
            # Assign it an Id
            distObj.doId = doId
            # Put the new do in the dictionary
            self.doId2do[doId] = distObj
            # Update the required fields
            distObj.generateInit()  # Only called when constructed
            distObj.generate()
            distObj.updateRequiredFields(dclass, di)
            # updateRequiredFields calls announceGenerate
            if wantOtpServer:
                print "New DO:%s, dclass:%s"%(doId, dclass.getName())
        return distObj

    def generateGlobalObject(self , doId, dcname):
        # Look up the dclass
        dclass = self.dclassesByName[dcname]
        # Create a new distributed object, and put it in the dictionary
        #distObj = self.generateWithRequiredFields(dclass, doId, di)
        
        # Construct a new one
        classDef = dclass.getClassDef()
        if classDef == None:
             self.notify.error("Could not create an undefined %s object." % (dclass.getName()))
        distObj = classDef(self)
        distObj.dclass = dclass
        # Assign it an Id
        distObj.doId = doId
        # Put the new do in the dictionary
        self.doId2do[doId] = distObj
        # Update the required fields
        distObj.generateInit()  # Only called when constructed
        distObj.generate()
        # updateRequiredFields calls announceGenerate
        return  distObj

    def generateWithRequiredOtherFields(self, dclass, doId, di):
        if self.doId2do.has_key(doId):
            # ...it is in our dictionary.
            # Just update it.
            distObj = self.doId2do[doId]
            assert(distObj.dclass == dclass)
            distObj.generate()
            distObj.updateRequiredOtherFields(dclass, di)
            # updateRequiredOtherFields calls announceGenerate
        elif self.cache.contains(doId):
            # ...it is in the cache.
            # Pull it out of the cache:
            distObj = self.cache.retrieve(doId)
            assert(distObj.dclass == dclass)
            # put it in the dictionary:
            self.doId2do[doId] = distObj
            # and update it.
            distObj.generate()
            distObj.updateRequiredOtherFields(dclass, di)
            # updateRequiredOtherFields calls announceGenerate
        else:
            # ...it is not in the dictionary or the cache.
            # Construct a new one
            classDef = dclass.getClassDef()
            if classDef == None:
                self.notify.error("Could not create an undefined %s object." % (dclass.getName()))
            distObj = classDef(self)
            distObj.dclass = dclass
            # Assign it an Id
            distObj.doId = doId
            # Put the new do in the dictionary
            self.doId2do[doId] = distObj
            # Update the required fields
            distObj.generateInit()  # Only called when constructed
            distObj.generate()
            distObj.updateRequiredOtherFields(dclass, di)
            # updateRequiredOtherFields calls announceGenerate
        return distObj


    def handleDisable(self, di):
        # Get the DO Id
        doId = di.getUint32()
        # disable it.
        self.disableDoId(doId)

    def disableDoId(self, doId):
         # Make sure the object exists
        if self.doId2do.has_key(doId):
            # Look up the object
            distObj = self.doId2do[doId]
            # remove the object from the dictionary
            del(self.doId2do[doId])

            # Only cache the object if it is a "cacheable" type
            # object; this way we don't clutter up the caches with
            # trivial objects that don't benefit from caching.
            if distObj.getCacheable():
                self.cache.cache(distObj)
            else:
                distObj.deleteOrDelay()
        else:
            ClientRepository.notify.warning(
                "Disable failed. DistObj "
                + str(doId) +
                " is not in dictionary")

    def handleDelete(self, di):
        # Get the DO Id
        doId = di.getUint32()
        self.deleteObject(doId)

    def deleteObject(self, doId):
        """
        Removes the object from the client's view of the world.  This
        should normally not be called except in the case of error
        recovery, since the server will normally be responsible for
        deleting and disabling objects as they go out of scope.

        After this is called, future updates by server on this object
        will be ignored (with a warning message).  The object will
        become valid again the next time the server sends a generate
        message for this doId.

        This is not a distributed message and does not delete the
        object on the server or on any other client.
        """
        if self.doId2do.has_key(doId):
            # If it is in the dictionary, remove it.
            obj = self.doId2do[doId]
            # Remove it from the dictionary
            del(self.doId2do[doId])
            # Disable, announce, and delete the object itself...
            # unless delayDelete is on...
            obj.deleteOrDelay()
        elif self.cache.contains(doId):
            # If it is in the cache, remove it.
            self.cache.delete(doId)
        else:
            # Otherwise, ignore it
            ClientRepository.notify.warning(
                "Asked to delete non-existent DistObj " + str(doId))

    def handleUpdateField(self, di):
        """
        This method is called when a CLIENT_OBJECT_UPDATE_FIELD
        message is received; it decodes the update, unpacks the
        arguments, and calls the corresponding method on the indicated
        DistributedObject.

        In fact, this method is exactly duplicated by the C++ method
        cConnectionRepository::handle_update_field(), which was
        written to optimize the message loop by handling all of the
        CLIENT_OBJECT_UPDATE_FIELD messages in C++.  That means that
        nowadays, this Python method will probably never be called,
        since UPDATE_FIELD messages will not even be passed to the
        Python message handlers.  But this method remains for
        documentation purposes, and also as a "just in case" handler
        in case we ever do come across a situation in the future in
        which python might handle the UPDATE_FIELD message.
        """
        # Get the DO Id
        doId = di.getUint32()
        #print("Updating " + str(doId))
        # Find the DO
            
        do = self.doId2do.get(doId)
        if (do != None):
            # Let the dclass finish the job
            do.dclass.receiveUpdate(do, di)
        else:
            ClientRepository.notify.warning(
                "Asked to update non-existent DistObj " + str(doId))

    def handleGoGetLost(self, di):
        # The server told us it's about to drop the connection on us.
        # Get ready!
        if (di.getRemainingSize() > 0):
            self.bootedIndex = di.getUint16()
            self.bootedText = di.getString()

            ClientRepository.notify.warning(
                "Server is booting us out (%d): %s" % (self.bootedIndex, self.bootedText))
        else:
            self.bootedIndex = None
            self.bootedText = None
            ClientRepository.notify.warning(
                "Server is booting us out with no explanation.")

    def handleServerHeartbeat(self, di):
        # Got a heartbeat message from the server.
        if base.config.GetBool('server-heartbeat-info', 1):
            ClientRepository.notify.info("Server heartbeat.")

    def handleSystemMessage(self, di):
        # Got a system message from the server.
        message = di.getString()
        self.notify.info('Message from server: %s' % (message))
        return message

    def handleUnexpectedMsgType(self, msgType, di):        
        if msgType == CLIENT_GO_GET_LOST:
            self.handleGoGetLost(di)
        elif msgType == CLIENT_HEARTBEAT:
            self.handleServerHeartbeat(di)
        elif wantOtpServer:
            if msgType == CLIENT_CREATE_OBJECT_REQUIRED:
                self.handleGenerateWithRequired(di)
            elif msgType == CLIENT_CREATE_OBJECT_REQUIRED_OTHER:
                self.handleGenerateWithRequiredOther(di)
            elif msgType == CLIENT_OBJECT_UPDATE_FIELD:
                self.handleUpdateField(di)
            elif msgType == CLIENT_OBJECT_DISABLE_RESP:
                self.handleDisable(di)
            elif msgType == CLIENT_OBJECT_DELETE_RESP:
                self.handleDelete(di)
            elif msgType == CLIENT_SYSTEM_MESSAGE:
                self.handleSystemMessage(di)
            elif msgType == CLIENT_CREATE_OBJECT_REQUIRED:
                self.handleGenerateWithRequired(di)
            elif msgType == CLIENT_CREATE_OBJECT_REQUIRED_OTHER:
                self.handleGenerateWithRequiredOther(di)
            elif msgType == CLIENT_DONE_SET_ZONE_RESP:
                self.handleSetZoneDone()                    
            elif msgType == CLIENT_OBJECT_LOCATION:
                self.handleObjectLocation(di)
        else:
            currentLoginState = self.loginFSM.getCurrentState()
            if currentLoginState:
                currentLoginStateName = currentLoginState.getName()
            else:
                currentLoginStateName = "None"
            currentGameState = self.gameFSM.getCurrentState()
            if currentGameState:
                currentGameStateName = currentGameState.getName()
            else:
                currentGameStateName = "None"
            ClientRepository.notify.warning(
                "Ignoring unexpected message type: " +
                str(msgType) +
                " login state: " +
                currentLoginStateName +
                " game state: " +
                currentGameStateName)

    def sendSetShardMsg(self, shardId):
        datagram = PyDatagram()
        # Add message type
        datagram.addUint16(CLIENT_SET_SHARD)
        # Add shard id
        datagram.addUint32(shardId)
        # send the message
        self.send(datagram)

    def sendSetZoneMsg(self, zoneId, visibleZoneList=None):
        datagram = PyDatagram()
        # Add message type
        datagram.addUint16(CLIENT_SET_ZONE)
        # Add zone id
        datagram.addUint32(zoneId)

        # if we have an explicit list of visible zones, add them
        if visibleZoneList is not None:
            vzl = list(visibleZoneList)
            vzl.sort()
            assert PythonUtil.uniqueElements(vzl)
            for zone in vzl:
                datagram.addUint32(zone)

        # send the message
        self.send(datagram)

    def handleDatagram(self, di):
        if self.notify.getDebug():
            print "ClientRepository received datagram:"
            di.getDatagram().dumpHex(ostream)

        msgType = self.getMsgType()
        
        # watch for setZoneDones
        if msgType == CLIENT_DONE_SET_ZONE_RESP:
            self.handleSetZoneDone()

        if self.handler == None:
            self.handleUnexpectedMsgType(msgType, di)
        else:
            self.handler(msgType, di)
            
        # If we're processing a lot of datagrams within one frame, we
        # may forget to send heartbeats.  Keep them coming!
        self.considerHeartbeat()
      
    if wantOtpServer:
        ##
        ##
        ## interest managment 
        ##
        ##
        def InterestAdd(self,  parentId, zoneId, Description):        
            """
            Part of the new otp-server code.
            """
            self.__interest_id_assign += 1
            self.__interesthash[self.__interest_id_assign] = Description
            contextId = self.__interest_id_assign;
            self.__sendAddInterest(contextId, parentId, zoneId)
            self.DumpInterests()
            return contextId;
            
        def InterestRemove(self,  contextId):        
            """
            Part of the new otp-server code.
            """
            answer = 0;
            if  self.__interesthash.has_key(contextId):
                self.__sendRemoveInterest(contextId)
                del self.__interesthash[contextId]
                answer = 1        
                        
            self.DumpInterests()            
            return answer


        def InterestAlter(self, contextId, parentId, zoneId, Description):        
            """
            Part of the new otp-server code.        
                Removes old and adds new.. 
            """
            answer = 0
            if  self.__interesthash.has_key(contextId):
                self.__interesthash[contextId] = Description
                self.__sendAlterInterest(contextId, parentId, zoneId)
                answer = 1
                
            self.DumpInterests()            
            return answer
            
        def DumpInterests(self):
            """
            Part of the new otp-server code.        
            """
            print "*********************** Interest Sets **************"
            for i in self.__interesthash.keys():
                 print "Interest ID:%s, Description=%s"%(i,self.__interesthash[i])
            print "****************************************************"

        
        def __sendAddInterest(self, contextId, parentId, zoneId):
            """
            Part of the new otp-server code.

            contextId is a client-side created number that refers to
                    a set of interests.  The same contextId number doesn't
                    necessarily have any relationship to the same contextId
                    on another client.
            """
            datagram = PyDatagram()
            # Add message type
            datagram.addUint16(CLIENT_ADD_INTEREST)
            datagram.addUint16(contextId)
            datagram.addUint32(parentId)
            datagram.addUint32(zoneId)
            self.send(datagram)

        def __sendAlterInterest(self, contextId, parentId, zoneId):
            """
            Part of the new otp-server code.

            contextId is a client-side created number that refers to
                    a set of interests.  The same contextId number doesn't
                    necessarily have any relationship to the same contextId
                    on another client.
            """
            datagram = PyDatagram()
            # Add message type
            datagram.addUint16(CLIENT_ALTER_INTEREST)
            datagram.addUint16(contextId)
            datagram.addUint32(parentId)
            datagram.addUint32(zoneId)
            self.send(datagram)

        def __sendRemoveInterest(self, contextId):
            """
            Part of the new otp-server code.

            contextId is a client-side created number that refers to
                    a set of interests.  The same contextId number doesn't
                    necessarily have any relationship to the same contextId
                    on another client.
            """
            datagram = PyDatagram()
            # Add message type
            datagram.addUint16(CLIENT_REMOVE_INTEREST)
            datagram.addUint16(contextId)
            self.send(datagram)



    def sendHeartbeat(self):
        datagram = PyDatagram()
        # Add message type
        datagram.addUint16(CLIENT_HEARTBEAT)
        # Send it!
        self.send(datagram)
        self.lastHeartbeat = globalClock.getRealTime()
        # This is important enough to consider flushing immediately
        # (particularly if we haven't run readerPollTask recently).
        self.considerFlush()

    def considerHeartbeat(self):
        """Send a heartbeat message if we haven't sent one recently."""
        if not self.heartbeatStarted:
            self.notify.debug("Heartbeats not started; not sending.")
            return
        
        elapsed = globalClock.getRealTime() - self.lastHeartbeat
        if elapsed < 0 or elapsed > self.heartbeatInterval:
            # It's time to send the heartbeat again (or maybe someone
            # reset the clock back).
            self.notify.info("Sending heartbeat mid-frame.")
            self.startHeartbeat()

    def stopHeartbeat(self):
        taskMgr.remove("heartBeat")
        self.heartbeatStarted = 0

    def startHeartbeat(self):
        self.stopHeartbeat()
        self.heartbeatStarted = 1
        self.sendHeartbeat()
        self.waitForNextHeartBeat()

    def sendHeartbeatTask(self, task):
        self.sendHeartbeat()
        self.waitForNextHeartBeat()
        return Task.done

    def waitForNextHeartBeat(self):
        taskMgr.doMethodLater(self.heartbeatInterval, self.sendHeartbeatTask,
                              "heartBeat")        
        
    def sendUpdate(self, do, fieldName, args, sendToId = None):
        dg = do.dclass.clientFormatUpdate(fieldName, sendToId or do.doId, args)
        self.send(dg)

    def replaceMethod(self, oldMethod, newFunction):
        return 0

    def getAllOfType(self, type):
        # Returns a list of all DistributedObjects in the repository
        # of a particular type.
        result = []
        for obj in self.doId2do.values():
            if isinstance(obj, type):
                result.append(obj)
        return result
