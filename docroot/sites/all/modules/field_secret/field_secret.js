(function ($) {
  Drupal.behaviors.fieldSecret = {
    attach: function(context, settings) {
      // The Form Widget Setup
      // Toggle the concealed/displayed elements in the node edit mode
      $('.form-field-secret-display-toggle').change(function() {
        var checkboxID = $(this).attr('id');
        var textDisplayClass = 'form-item' + checkboxID.substr(4, checkboxID.length - 20) + 'value';
        var textPassClass = 'form-item' + checkboxID.substr(4, checkboxID.length - 20) + 'dummy-pass';
        $('.' + textDisplayClass + ',.' + textPassClass).toggle();
      });
      
      // Prefill the concealed secret field with asterisks if there is already a value
      $('.field-widget-field-secret-fancy input[type=password]').each(function() {
        var passFieldID = $(this).attr('id');
        // Replacing the last 'dummy-pass' with 'value'
        var realFieldID = passFieldID.substr(0, passFieldID.length - 10) + 'value';
        $(this).val($('#' + realFieldID).val());
      });
      
      // Sync the secret field to the concealed field
      $('.field-widget-field-secret-fancy input[type=password]').keyup(function() {
        var passFieldID = $(this).attr('id');
        // Replacing the last 'dummy-pass' with 'value'
        var realFieldID = passFieldID.substr(0, passFieldID.length - 10) + 'value';
        $('#' + realFieldID).val($(this).val());
      });
      
      // Sync the concealed field to the secret field
      $('.form-field-secret-concealed input').keyup(function() {
        var realFieldID = $(this).attr('id');
        // Replacing the last 'value' with 'dummy-pass'
        var passFieldID = realFieldID.substr(0, realFieldID.length - 5) + 'dummy-pass';
        $('#' + passFieldID).val($(this).val());
      });
      
      // ZeroClipboard processing for the copy button in the node edit mode
      Drupal.zeroClipboard.process('.form-field-secret-copy-text', function(elementID) {
        elementID = elementID.substr(0, elementID.length - 11) + 'value';
        return $('#' + elementID).val();
      });
      
      // The View Formatter Setup
      // Toggle the concealed/displayed elements in the node view mode
      $('.field-secret-display-toggle').change(function() {
        $(".field-secret-text-asterisks, .field-secret-text-displayed", '#field-secret-text-wrapper--' + $(this).attr('rel')).toggle();
      });
      
      // ZeroClipboard processing for the copy button in the node view mode
      Drupal.zeroClipboard.process('.field-secret-copy-text', function(elementID) {
        return $('.field-secret-text-displayed', $('#' + elementID).parents('.field-item')).html();
      });
    }
  };
})(jQuery);