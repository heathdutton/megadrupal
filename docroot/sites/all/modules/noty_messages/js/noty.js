var notyMessages = notyMessages || {};

(function($) {
  Drupal.behaviors.notyMessagesRenderMessages = {
    attach: function(context, settings) {
      // Don't deal with config here.
      for (var type in settings.notyMessagesNoties){
        // Pre construct the settings for noty.
        notyMessages.renderType(type, settings.notyMessagesNoties[type], settings.notyMessages);
      }
    }
  }
  notyMessages.renderType = function(typeName, typeData, config){
    var notySettings  = {
      layout: config.notyLayout[typeName],
      type: config.notyType[typeName],
      textAlign: config.notyTextAlign[typeName],
      speed: config.notySpeed[typeName],
      timeout: parseInt(config.notyTimeout[typeName], 10),
      closable: config.notyClosable[typeName],
      closeOnSelfClick: config.notyClickClose[typeName],
      modal: config.notyModal[typeName],
      theme: config.notyTheme[typeName]
    }
    for (message in typeData){
      // Set the text.
      notySettings.text = typeData[message];
      noty(notySettings);
    }
  }
})(jQuery);
