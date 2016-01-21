(function($) {
  Drupal.behaviors.youtube_player_accessibility = {
    attach: function(context, settings) {
      requirementsTxt = settings.YoutubePA.requirementsTxt;
      headerTxt = settings.YoutubePA.headerTxt;
      playTxt = settings.YoutubePA.playTxt;
      forwardTxt = settings.YoutubePA.forwardTxt;
      backTxt = settings.YoutubePA.backTxt;
      stopTxt = settings.YoutubePA.stopTxt;
      volUpTxt = settings.YoutubePA.volUpTxt;
      volDnTxt = settings.YoutubePA.volDnTxt;
      muteTxt = settings.YoutubePA.muteTxt;
      loopTxt = settings.YoutubePA.loopTxt;
      currentlyPlayingTxt = settings.YoutubePA.currentlyPlayingTxt;
      timeTxt = settings.YoutubePA.timeTxt;
      unmuteTxt = settings.YoutubePA.unmuteTxt;
      pauseTxt = settings.YoutubePA.pauseTxt;
      unloopTxt = settings.YoutubePA.unloopTxt;
      videoPlayListTxt = settings.YoutubePA.videoPlayListTxt;
      ofTxt = settings.YoutubePA.ofTxt;
    }
  }
})(jQuery);

/* Saddly Overriding the function because it's not genrice */

function ytPlayerBoxDraw(aspect,ytpbox,pid) {
  var width = "480px";
  if (aspect == "normal") {
    width = "480px";
  }

  else if (aspect == "wide") {
    width = "640px";
  }

  if (ytpbox) {
    ytpbox.style.width = width;
    ytpbox.innerHTML =  "<div id=\"ytapiplayer"+ pid +"\">" +
                          requirementsTxt +
                        "<\/div>" +
                        "<h3 class=\"semantic\">"+headerTxt+"<\/h3>" +
                        "<ul class=\"ytplayerbuttons\">" +
                          "<li><a id=\"ytplaybut"+ pid +"\" href=\"/\">"+playTxt+"<\/a><\/li>" +
                          "<li><a id=\"ytforwardbut"+ pid +"\" href=\"/\">"+forwardTxt+"<\/a><\/li>" +
                          "<li><a id=\"ytbackbut"+ pid +"\" href=\"/\">"+backTxt+"<\/a><\/li>" +
                          "<li><a id=\"ytstopbut"+ pid +"\" href=\"/\">"+stopTxt+"<\/a><\/li>" +
                          "<li><a id=\"ytvolupbut"+ pid +"\" href=\"/\">"+volUpTxt+"<\/a><\/li>" +
                          "<li><a id=\"ytvoldownbut"+ pid +"\" href=\"/\">"+volDnTxt+"<\/a><\/li>" +
                          "<li><a id=\"ytmutebut"+ pid +"\" href=\"/\">"+muteTxt+"<\/a><\/li>" +
                          "<li><a id=\"ytloopbut"+ pid +"\" href=\"/\">"+loopTxt+"<\/a><\/li>" +
                        "<\/ul>" +
                        "<h4>"+currentlyPlayingTxt+" <span id=\"ytvidtitle"+ pid +"\"><\/span><\/h4>" +
                        "<h4>"+timeTxt+" <span id=\"ytplayertime"+ pid +"\"><\/span><\/h4>";
  }
}

function ytloop(ytpid) {
  var ytp = document.getElementById("thisytp"+ytpid);
  if (new RegExp('^(' + ytplayer.join('|') + ')$').test(ytp.id)) {
    var loopbut = document.getElementById("ytloopbut"+ytpid);
    if (loopbut.firstChild.nodeValue == loopTxt) {
      loopbut.firstChild.nodeValue = unloopTxt;
      ytLoopInterval[ytpid] = window.setInterval(function() { ytp.playVideo(); }, 2000);
    }
    else {
      loopbut.firstChild.nodeValue = loopTxt;
      clearInterval(ytLoopInterval[ytpid]);
    }
  }
}

function updateButtonState(ytpid) {
  var ytp = document.getElementById("thisytp"+ytpid);
  if (new RegExp('^(' + ytplayer.join('|') + ')$').test(ytp.id)) {

    var mutebut = document.getElementById("ytmutebut"+ ytpid);
    var playbut = document.getElementById("ytplaybut"+ ytpid);
    if (ytp.isMuted()) {
      mutebut.firstChild.nodeValue = unmuteTxt;
    }
    else {
      mutebut.firstChild.nodeValue = muteTxt;
    }

    if (ytp.getPlayerState() == 1) {
      playbut.firstChild.nodeValue = pauseTxt;
    }

    if (ytp.getPlayerState() == 2) {
      playbut.firstChild.nodeValue = playTxt;
    }
  }
}