(function ($) {
  // Store our function as a property of Drupal.behaviors.
        Drupal.behaviors.transform = {
  attach:function (context, settings) {
    // Apply transformation if "Enable tranformation" is set
    if (Drupal.settings.transform.enabled) {
      class_id = Drupal.settings.transform.class_id;
      form_list = Drupal.settings.transform.form_list;
      exclude_class_id = Drupal.settings.transform.exclude_class_id;
      exclude_form_list = Drupal.settings.transform.exclude_form_list;
      select_width = parseInt(Drupal.settings.transform.select_width);
      textfield_width = parseInt(Drupal.settings.transform.textfield_width);
      
      // Prefix (class or ID) for form that will get transformed
      if (class_id == 0) {
        prefix = '.';
      }
      else if (class_id == 1) {
        prefix = '#';
      }
      // Prefix (class or ID) for form that won't get transformed
      if (exclude_class_id == 0) {
        prefix_exclude = '.';
      }
      else if (exclude_class_id == 1) {
        prefix_exclude = '#';
      }

      // Generate an array with all forms to transform
      form_list = $.trim(form_list);
      if (form_list.length > 0) {
        forms = form_list.split('\r\n');
      }
      else {
        forms = false;
      }
      // Generate an array with all forms to exclude from  being transformed
      exclude_form_list = $.trim(exclude_form_list);
      if (exclude_form_list.length > 0) {
        forms_exclude = exclude_form_list.split('\r\n');
      }
      else {
        forms_exclude = false;  
      }

      // Exclude forms
      if (forms_exclude) {
        $.each(forms_exclude, function(index, value){
          form = "form" + prefix_exclude + value;
          $(form).addClass("jqtransformdone");
        });
      }
      // Transform forms
      if (forms) {
        $.each(forms, function(index, value){
          form = "form" + prefix + value;
          // Transform the form, if it is not excluded
          $(form).jqTransform();
          // Set a min-width for the selectboxes
          if (select_width) {
            $(form + " .jqTransformSelectWrapper").css('min-width', select_width);
            $(form + " .jqTransformSelectWrapper span").css('min-width', (select_width - 31));
            $(form + " .jqTransformSelectWrapper ul").css('min-width', (select_width - 2));
          }
          // Set a min-width for the textfields
          if (textfield_width) {
            $(form + " .jqTransformInputWrapper").css('width', textfield_width);
            $(form + " .jqTransformInputWrapper input").css('width', (textfield_width - 10));
          }
          
          // Prevents gripper from be applied to the textarea
          $(form + " textarea").addClass('textarea-processed');
        });
        
      }
      else {
        // Transform all forms, but not those that is excluded
        $("form").jqTransform();
      }
    }
  }
    }
  }(jQuery));