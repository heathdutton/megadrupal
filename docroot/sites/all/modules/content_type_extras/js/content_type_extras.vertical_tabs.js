(function ($) {
Drupal.behaviors.content_type_extras_vertical_tabs = {
  attach: function (context) {
    $('fieldset#edit-submission', context).drupalSetSummary(function (context) {
      var vals = [];
      
      var title = $('#edit-title-label').val();
      title = Drupal.t('Title: @title' , { '@title': title });
      
      // Preview
      var preview = $('#edit-node-preview input[type=radio]:checked').val();
      if (preview != 0) {
        vals.push(Drupal.t('Preview'));
      }
      
      // Save and New
      var save_and_new = $('#edit-node-save-and-new input[type=radio]:checked').val();
      if (save_and_new != 0) {
        vals.push(Drupal.t('Save and New'));
      }
      
      // Save and Edit
      var save_and_edit = $('#edit-node-save-and-edit input[type=radio]:checked').val();
      if (save_and_edit != 0) {
        vals.push(Drupal.t('Save and Edit'));
      }
      
      // Cancel
      var cancel = $('#edit-node-cancel input[type=radio]:checked').val();
      if (cancel != 0) {
        vals.push(Drupal.t('Cancel'));
      }
      
      var options = '';
      if (vals.length > 0) {
        options = Drupal.t('Options: @options', { '@options': vals.join(', ')});
      }
      
      return title + '<br>' + options;
    });
    
    // Copied from /modules/node/content_types.js
    $('fieldset#edit-workflow', context).drupalSetSummary(function(context) {
      var vals = [];
      $("input[name^='node_options']:checked", context).parent().each(function() {
        vals.push(Drupal.checkPlain($(this).text()));
      });
      if (!$('#edit-node-options-status', context).is(':checked')) {
        vals.unshift(Drupal.t('Not published'));
      }
      return vals.join(', ');
    });
    $('fieldset#edit-display', context).drupalSetSummary(function(context) {
      var vals = [];
      $('input:checked', context).next('label').each(function() {
        vals.push(Drupal.checkPlain($(this).text()));
      });
      if (!$('#edit-node-submitted', context).is(':checked')) {
        vals.unshift(Drupal.t("Don't display post information"));
      }
      return vals.join(', ');
    });
    
    // Copied from xmlsitemap/xmlsitemap.js
    $('fieldset#edit-xmlsitemap', context).drupalSetSummary(function (context) {
      var vals = [];
      // Inclusion select field.
      var status = $('#edit-xmlsitemap-status option:selected').text();
      vals.push(Drupal.t('Inclusion: @value', { '@value': status }));
      // Priority select field.
      var priority = $('#edit-xmlsitemap-priority option:selected').text();
      vals.push(Drupal.t('Priority: @value', { '@value': priority }));
      return vals.join('<br />');
    });
    
    $('fieldset#edit-extras', context).drupalSetSummary(function (context) {
      var vals = [];
      $('input:checked', context).next('label').each(function() {
        vals.push(Drupal.checkPlain($(this).text()));
      });
      return vals.join(', ');
    });
  }
};
})(jQuery);
