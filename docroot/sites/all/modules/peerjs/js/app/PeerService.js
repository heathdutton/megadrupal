/**
 * @file
 * Peer service.
 */

(function () {
  'use strict';

  angular.module('PeerJSChat')
    .factory('PeerService', function ($filter, $interval, $sce, Settings, EntityService) {
      var service = {
        settings: Settings,
        pid: Settings.user.pid,
        Peer: null,
        localStream: null,
        peers: [],
        mediaConnections: [],
        dataConnections: [],
        messages: [],
        peersInterval: null,
        formData: {
          status: localStorage.getItem('peerStatus') || 1,
          videoChatStatus: 0
        }
      };

      /**
       * Initialize controller.
       */
      service.changeStatus = function (status) {
        service.formData.status = status;
        localStorage.setItem('peerStatus', status);
        if (status) {
          service.createPeerConnection();
        }
        else {
          service.destroyPeerConnection();
        }
      };

      /**
       * Create the Peer connection.
       */
      service.createPeerConnection = function () {
        service.Peer = new Peer(null, Settings.options);

        // On open connection to the PeerServer.
        service.Peer.on('open', function () {

          // Add user to this chat room. Creates a new peer in the
          // peerjs_peer table.
          EntityService.get({
            entity_type: 'peerjs_peer',
            cid: Settings.entity.cid,
            author: Settings.user.uid
          })
            .$promise.then(function (data) {

              // Create a new peer entry if it doesn't exist.
              if (data.list.length === 0) {
                EntityService.save({entity_type: 'peerjs_peer'}, {
                  pid: service.Peer.id,
                  author: Settings.user.uid,
                  cid: Settings.entity.cid
                });
              }

              // Otherwise update the old entry.
              else if (data.list.length === 1) {
                EntityService.update({
                  entity_type: 'peerjs_peer',
                  id: data.list[0].id
                }, {
                  pid: service.Peer.id
                });
              }
            }, function (error) {
              console.log(error);
            });

          // Attempt to establish a data connection with each peer.
          service.connectPeers();
        });

        // On close connection to the PeerServer.
        service.Peer.on('close', function () {
          service.mediaConnections = [];
          service.dataConnections = [];
          service.calls = [];
          service.messages = [];
        });

        // On disconnect from the PeerServer.
        service.Peer.on('disconnected', function (peer) {
        });

        // Handle incoming calls.
        service.Peer.on('call', function (mediaConnection) {
          console.log("CALL RECEIVED:", mediaConnection);
          if (mediaConnection.metadata.type === 'public') {
            service.publicAnswer(mediaConnection);
          }
        });

        // Handle incoming data connections.
        service.Peer.on('connection', function (dataConnection) {

          // Only connect to new peers.
          if ($filter('filter')(service.dataConnections, {'peer': dataConnection.peer}).length === 0) {
            service.initializeDataConnection(dataConnection);
          }
          else {
            dataConnection.close();
          }
        });

        // Handle errors.
        service.Peer.on('error', function (error) {
          if (error.type === 'browser-incompatible') {
            service.changeStatus(0);
            alert(error.message + '. Please try Google Chrome or Firefox instead.');
          }
        });

        // Send a heartbeat to Peer Server to keep the connection open.
        service.peersInterval = $interval(function () {
          if (service.Peer.socket._wsOpen()) {
            service.Peer.socket.send({type: 'HEARTBEAT'});
            service.connectPeers();
            if (service.formData.videoChatStatus) {
              service.publicCall();
            }
          }
        }, 5000);
      };

      /**
       * Close the Peer connection.
       */
      service.destroyPeerConnection = function () {
        if (service.Peer !== null) {
          service.Peer.destroy();
          service.Peer = null;
          service.peers = [];
          service.mediaConnections = [];
          service.dataConnections = [];
          service.messages = [];
          $interval.cancel(service.peersInterval);
          service.stopLocalMediaStream();
          service.formData.videoChatStatus = false;
        }
      };

      /**
       * Initialize controller.
       */
      service.connectPeers = function () {
        EntityService.get({
          entity_type: 'peerjs_peer',
          cid: Settings.entity.cid
        })
          .$promise.then(function (data) {
            service.peers = $filter('filter')(data.list, {'uid': '!' + Settings.user.uid});

            // Iterate over each peer and try to connect to peer.
            data.list.forEach(function (peer) {

              // Only try to connect to new peers.
              if ($filter('filter')(service.dataConnections, {'peer': peer.pid}).length === 0) {
                service.initializeDataConnection(service.Peer.connect(peer.pid));
              }
            });
          });
      };

      /**
       * Create a dataConnection with a peer.
       */
      service.initializeDataConnection = function (dataConnection) {

        // Handle dataConnection open event.
        dataConnection.on('open', function () {

          // Only add this connection if it doesn't already exist.
          if ($filter('filter')(service.dataConnections, {'peer': this.peer}).length === 0) {

            // Let peer know that connection was successful, and send user info.
            this.send({
              type: 'status',
              user: Settings.user
            });
          }
        });

        // Handle dataConnection close event.
        dataConnection.on('close', function () {
          var index = service.dataConnections.indexOf(this);
          service.dataConnections.splice(index, 1);
        });

        // Handle dataConnection data event.
        dataConnection.on('data', function (data) {
          if (data.type === 'status') {
            this.metadata = {
              user: data.user
            };
            service.dataConnections.push(this);
          }
          if (data.type === 'public') {
            service.messages.push(data);
          }
        });
      };

      /**
       * Create a dataConnection with a peer.
       */
      service.message = function () {
        var message = {
          type: 'public',
          user: Settings.user,
          text: service.formData.messageText
        };

        service.messages.push(message);
        service.dataConnections.forEach(function (dataConnection) {
          message.peer = dataConnection.peer;
          dataConnection.send(message);
        });

        service.formData.messageText = '';
      };

      /**
       * Initialize controller.
       */
      service.updateStatus = function () {
        service.dataConnections.forEach(function (dataConnection) {
        });
      };

      /**
       * Enable local media stream.
       */
      service.startLocalMediaStream = function (cb) {
        service.stopLocalMediaStream();
        navigator.getUserMedia(
          Settings.constraints,
          function (stream) {
            service.localStream = stream;
            service.localStreamUrl = $sce.trustAsResourceUrl(URL.createObjectURL(stream));
            if (angular.isFunction(cb)) {
              cb();
            }
          },
          function (error) {
            console.log(error);
          }
        );
      };

      /**
       * Disable local media stream.
       */
      service.stopLocalMediaStream = function () {
        if (service.localStream !== null) {
          service.localStream.stop();
          service.localStream = null;
          service.localStreamUrl = '';
        }
      };

      /**
       * Call a peer.
       */
      service.joinVideoChat = function () {
        service.formData.videoChatStatus = confirm('Are you sure you want to join video chat? Your webcam video will be broadcast to all members in this chat room.');
        if (service.formData.videoChatStatus) {
          service.publicCall();
        }
      };

      /**
       * Call a peer.
       */
      service.leaveVideoChat = function () {
        service.formData.videoChatStatus = !confirm('Are you sure you want to leave video chat?');
        if (!service.formData.videoChatStatus) {
          service.publicHangup();
        }
      };

      /**
       * Call a peer.
       */
      service.publicCall = function () {
        if (service.localStream === null) {
          service.startLocalMediaStream(function () {
            service.publicCall();
          });
          return;
        }
        service.dataConnections.forEach(function (dataConnection) {
          var filteredMediaConnections = $filter('filter')(service.mediaConnections, {'peer': dataConnection.peer, metadata: {user: {uid: Settings.user.uid}}});
          if (filteredMediaConnections.length === 0) {
            var mediaConnection = service.Peer.call(dataConnection.peer, service.localStream, {
              metadata: {
                type: 'public',
                user: Settings.user
              }
            });
            service.initializeMediaConnection(mediaConnection);
          }
        });
      };

      /**
       * Call a peer.
       */
      service.publicHangup = function () {
        var filteredMediaConnections = $filter('filter')(service.mediaConnections, {metadata: {user: {uid: Settings.user.uid}}});
        filteredMediaConnections.forEach(function (mediaConnection) {
          mediaConnection.close();
        });
        service.stopLocalMediaStream();
      };

      /**
       * Answer a public call.
       */
      service.publicAnswer = function (mediaConnection) {
        var filteredMediaConnections = $filter('filter')(service.mediaConnections, {'peer': mediaConnection.peer, answered: true, open: true});
        if (filteredMediaConnections.length === 0) {
          mediaConnection.answered = true;
          mediaConnection.answer(null);
          service.initializeMediaConnection(mediaConnection);
          console.log("ANSWERED:", mediaConnection);
        }
        else {
          console.log("NOT ANSWERED:", mediaConnection);
        }

      };

      /**
       * Create a dataConnection with a peer.
       */
      service.initializeMediaConnection = function (mediaConnection) {

        // Handle mediaConnection stream event.
        mediaConnection.on('stream', function (stream) {
        });

        // Handle mediaConnection close event.
        // Firefox doesn't support this event.
        mediaConnection.on('close', function () {
          console.log("MEDIA CONNECTION CLOSED");
          var index = service.mediaConnections.indexOf(mediaConnection);
          service.mediaConnections.splice(index, 1);
        });

        // Handle mediaConnection close event.
        mediaConnection.on('error', function (err) {
          console.log(err);
        });

        service.mediaConnections.push(mediaConnection);
      };
      return service;
    });
}());
