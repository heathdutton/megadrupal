(function($){
  Drupal.workflowFields = {
    select: function(selector, group) {
      if (group == 'all') {
        this.select(selector, 'visible');
        this.select(selector, 'editable');
        return;
      }
      switch (selector) {
        case 'all':
          $('.workflow-fields-table input[type=checkbox][id*='+group+']').attr('checked', 'true');
          break;
        case 'none':
          $('.workflow-fields-table input[type=checkbox][id*='+group+']').removeAttr('checked');
          break;
        case 'toggle':
          $('.workflow-fields-table input[type=checkbox][id*='+group+']').each(function() {
            if (this.checked) $(this).removeAttr('checked');
            else this.checked = true;
          });
          break;
        default:
          // Match against selectors (role ids) in the form '-'+rid 
          // but not '--'+rid to differentiate between rid=1 and rid=-1
          $('.workflow-fields-table input[type=checkbox][value='+selector+'][id*='+group+']').each(function() {
            if (this.checked) $(this).removeAttr('checked');
            else this.checked = true;
          });
      }
    }
  };
})(jQuery);