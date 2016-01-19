/**
 * @file
 * The JavaScript handler for osCaddie Alfresco Model Mapping.
 */

(function ($) {
  Drupal.behaviors.oscaddie_alfrescoModelMapping = {
    attach: function (context, settings) {
      $('#mapping-fieldset').hide();

      $('input:radio[name*="drupal"]').click(function() {
        $('#mapping-fieldset').show();

        $("input:radio").attr('checked', false);
        $(this).attr('checked', true);
        $('#edit-mapping-drupal-label-wrapper .form-item span').html($(this).parent().text());

        if ($('input:radio[name*="drupal"]:checked').val()) {
          $('#edit-mapping-drupal-select-wrapper').hide();
          $('#edit-mapping-drupal-label-wrapper').show();
          $('#edit-mapping-alfresco-label-wrapper').hide();
          //$('#edit-mapping-alfresco-label').val('');
          $('#edit-mapping-alfresco-select-wrapper').show();
        }
      });

      $('input:radio[name*="alfresco"]').click(function() {
        $('#mapping-fieldset').show();

        $("input:radio").attr('checked', false);
        $(this).attr('checked', true);
        $('#edit-mapping-alfresco-label-wrapper .form-item span').html($(this).parent().text());

        if ($('input:radio[name*="alfresco"]:checked').val()) {
          $('#edit-mapping-drupal-select-wrapper').show();
          $('#edit-mapping-drupal-label-wrapper').hide();
          //$('#edit-mapping-drupal-type-field-text').val('');
          $('#edit-mapping-alfresco-label-wrapper').show();
          $('#edit-mapping-alfresco-select-wrapper').hide();
        }
      });
    }
  }
})(jQuery);
