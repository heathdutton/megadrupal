<?php

/**
 * @file
 * This template handles the overall display of the PeerJS chat room.
 *
 * Any overrides to this template must preserve angular attributes, otherwise
 * the app will not function properly.
 */
?>

<div class="peerjs flex-container" ng-app="PeerJSChat" ng-controller="PeerJSChatCtrl" ng-init="init()">

  <!-- Toolbar -->
  <div class="peerjs-toolbar flex-item">
    <div ng-if="peerService.formData.status == 0">
      <button class="join" ng-click="peerService.changeStatus(1)">Join Chatroom</button>
    </div>
    <div ng-if="peerService.formData.status == 1">
      <button class="leave" ng-click="peerService.changeStatus(0)" ng-if="peerService.formData.status == 1">Leave Chatroom</button>
      <button ng-if="!peerService.formData.videoChatStatus" ng-click="peerService.joinVideoChat()">Enable Video</button>
      <button ng-if="peerService.formData.videoChatStatus" ng-click="peerService.leaveVideoChat()">Disable Video</button>
    </div>
  </div>

  <!-- Peer List -->
  <ul class="peerjs-peers flex-item" ng-if="peerService.formData.status == 1">

    <!-- Local User -->
    <li class="peerjs-peer">

      <!-- User picture -->
      <div class="peerjs-user-picture" ng-if="user.picture">
        <img src="{{user.picture}}" alt="User Picture"/>
      </div>

      <!-- User name -->
      <div class="peerjs-user-name"><span class="truncate">{{user.realname}}</span></div>
    </li>

    <!-- Remote Peers - theme/peerjs--peer.tpl.php -->
    <li class="peerjs-peer" ng-repeat="dataConnection in peerService.dataConnections" ng-controller="DataConnectionCtrl" ng-init="init()" ng-if="peerService.dataConnections.length">

      <!-- User picture -->
      <div class="peerjs-user-picture" ng-if="user.picture">
        <img src="{{user.picture}}" alt="User Picture"/>
      </div>

      <!-- User name -->
      <div class="peerjs-user-name"><span class="truncate">{{user.realname}}</span></div>
    </li>
  </ul>

  <!-- Chat (Video and Text) -->
  <div class="peerjs-chat flex-item" ng-if="peerService.formData.status == 1">

    <!-- Video/Audio Calls -->
    <div class="peerjs-calls">
      <ul ng-if="peerService.mediaConnections.length || (peerService.formData.videoChatStatus && peerService.localStreamUrl)">

        <!-- Local User -->
        <li class="peerjs-call" ng-if="peerService.formData.videoChatStatus">
          <div class="thumbnail">
            <div class="peerjs-call-video" ng-if="peerService.localStreamUrl">
              <video universal-muted style="width:100%" ng-src="{{peerService.localStreamUrl}}" autoplay></video>
            </div>
            <div class="caption">{{user.realname}}</div>
          </div>
        </li>

        <!-- Remote Users-->
        <li class="peerjs-call" ng-repeat="mediaConnection in peerService.mediaConnections | filter:{metadata:{type:'public'},answered:true}" ng-controller="MediaConnectionCtrl" ng-init="init()" ng-show="mediaConnection.open == true">
          <pre>{{remoteStreamUrl}}</pre>
          <div class="thumbnail">
            <div class="peerjs-call-video" ng-if="!mediaConnection.remoteStream.ended && remoteStreamUrl !== ''">
              <video style="width:100%" ng-src="{{remoteStreamUrl}}" autoplay></video>
            </div>
            <div class="caption">{{user.realname}}</div>
          </div>
        </li>
      </ul>
    </div>

    <!-- Messages -->
    <div class="peerjs-messages">
      <ul scroll-bottom="peerService.messages" min-height="100" max-height="300">
        <li class="peerjs-message" ng-repeat="message in peerService.messages">
          {{message.user.realname}}: {{message.text}}
        </li>
      </ul>

      <form ng-submit="peerService.message()" role="form">
        <input type="text" placeholder="Type a message..." ng-model="peerService.formData.messageText">
        <button type="button" ng-click="peerService.message()">Send</button>
      </form>
    </div>
  </div>
</div>

<!--<pre>MEDIA CONNECTIONS: {{peerService.mediaConnections.length}}</pre>-->
<!--<ul class="list-group">-->
<!--<li class="peerjs-call list-group-item" ng-repeat="mediaConnection in peerService.mediaConnections">-->
<!--<pre>ID: {{mediaConnection.id}}</pre>-->
<!--<pre>PEER: {{mediaConnection.peer}}</pre>-->
<!--<pre>USER: {{mediaConnection.metadata}}</pre>-->
<!--<pre>OPEN: {{mediaConnection.open}}</pre>-->
<!--</li>-->
<!--</ul>-->
<!--<pre>DATA CONNECTIONS: {{dataConnections.length}}</pre>-->
<!--<ul class="list-group">-->
<!--<li class="peerjs-call list-group-item" ng-repeat="dataConnection in peerService.dataConnections">-->
<!--<pre>ID: {{dataConnection.id}}</pre>-->
<!--<pre>PEER: {{dataConnection.peer}}</pre>-->
<!--<pre>USER: {{dataConnection.metadata}}</pre>-->
<!--<pre>OPEN: {{dataConnection.open}}</pre>-->
<!--</li>-->
<!--</ul>-->
