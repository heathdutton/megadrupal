/**
 * @file
 * Attaches behaviors for the Block Inject module.
 */

(function ($) {

Drupal.behaviors.blockInjectFieldsetSummaries = {
  attach: function (context) {
 
    $('fieldset.block-inject-node-form', context).drupalSetSummary(function (context) {
      
      var vals = [];
      
      // The exception checkbox.
      if ($('.form-item-block-inject-exception input').is(':checked')) {
        vals.push(Drupal.t('No injection'));
      }
      else {
        vals.push(Drupal.t('Not excepted'));
      }
   
      // The offset select.      
      var offset = $('.form-item-block-inject-offset select').val();
      
      if (offset=='0') {
        vals.push(Drupal.t('No offset'));
      }
      else {
        if (offset=='1' || offset=='-1') {
          vals.push(Drupal.t('Offset of ' + offset + ' paragraph'));
        }
        else {
          vals.push(Drupal.t('Offset of ' + offset + ' paragraphs'));
        }
      }
      
      return vals.join(', ');
      
    });
    
  }
};

})(jQuery);
