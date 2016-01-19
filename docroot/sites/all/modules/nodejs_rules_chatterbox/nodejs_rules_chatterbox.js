// JavaScript Document
(function ($) {


Drupal.nodejsRulesChatterbox = Drupal.nodejsRulesChatterbox || {'initialised' : false};
/**
 * Initialize chatterbox.
 */
Drupal.nodejsRulesChatterbox.initializeBoard = function(channel) {
  $('#chatterbox-board-' + channel).scroll(function(e) {
    var yPos = $('#chatterbox-board-' + channel).scrollTop() + $('#chatterbox-board-' + channel).innerHeight();
	e.preventDefault();
    e.stopPropagation();
	height = $('#chatterbox-board-' + channel)[0].scrollHeight;
    if (yPos >=height) {
	  var id = $( "#chatterbox-board-" + channel + " .nodejs-rules-chatterbox-message" ).last().attr("id");
	  var msid = id.replace(/^chatterbox-message-/, '');
      Drupal.nodejsRulesChatterbox.getPreviousMessages(channel, msid);
    }
  });
  Drupal.nodejsRulesChatterbox.initializeMessages(channel);
}

/**
 * Nodejs callback handler.
 */
Drupal.Nodejs.callbacks.nodejsRulesChatterboxMessageHandler = {
  callback: function (message) {
    Drupal.nodejsRulesChatterbox.addMessageToBoard(message.data);
  }
};

/**
 * Add message to chatterbox.
 */
Drupal.nodejsRulesChatterbox.addMessageToBoard = function(message) {
  if($('#chatterbox-board-' + message.channel + " .chatterbox-message-unavilable").length) {
    $('#chatterbox-board-' + message.channel + " .chatterbox-message-unavilable").remove();
  }
  $('#chatterbox-board-' + message.channel).prepend(message.body);
  $('#chatterbox-message-' + message.id).hide();
  $('#chatterbox-message-' + message.id).show('normal');
  $('#chatterbox-board-' + message.channel).animate({scrollTop: "0px"}, 1000);
};

/**
 * Update previous messages while scrolling.
 */
Drupal.nodejsRulesChatterbox.getPreviousMessages = function(channel,msid) {
 var channel = channel;
 $.post(
    Drupal.settings.nodejs_rules_chatterbox.previousMessagePath,
    {"channel": channel,"msid": msid},
	function(response){
	  if(response[1] !== undefined && response[1].data != '') {
		$('#chatterbox-board-' + channel).append(response[1].data);
      }
	},
	'json'
 );
}

/**
 * Initialize chatter box messages.
 */
Drupal.nodejsRulesChatterbox.initializeMessages = function(channel) {
 var channel = channel;
 $.post(
    Drupal.settings.nodejs_rules_chatterbox.initializesMessagePath,
    {"channel": channel,"start":0},
	function(response){
	  if(response[1] !== undefined && response[1].data != '') {
		$('#chatterbox-board-' + channel).animate({scrollTop: "0px"}, 1000);
		$('#chatterbox-board-' + channel).append(response[1].data);
      }
	},
	'json'
 );
}

})(jQuery);