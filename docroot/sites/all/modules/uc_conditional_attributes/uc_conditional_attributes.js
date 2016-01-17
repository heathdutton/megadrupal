/**
 * Allows uc_conditional_attributes to be notified when the pages loads and the
 * DOM is ready, or when any changes to the DOM are made. When this happens,
 * we will need to rebuild the display immediately and then bind ourself to any
 * future onchange events.
 */
Drupal.behaviors.uc_conditional_attributes = {
  attach: function(context, settings) {

    jQuery('input.uc_conditional_attributes_flag', context).each(function() {
      var form_context = jQuery(this).parents('form');
      uc_conditional_attributes_rebuild_attribute_display(form_context);
      uc_conditional_attributes_bind_attribute_display(form_context);
    });
  }
};

function uc_conditional_attributes_debug(message) {
  var local_debug = false;
  if (Drupal.settings.uc_conditional_attributes.debug || local_debug) {
    console.log(message);
  }
}

/**
 * Rebuilds the display immediately, use this when DOM changes for example.
 */
function uc_conditional_attributes_rebuild_attribute_display(context) {
  uc_conditional_attributes_debug('\n\n');
  jQuery('.uc-conditional-attributes-dependent-attr', context).each(function() {
    jQuery(this).parents(".form-item").parents(".attribute").hide();
  });

  jQuery(".attributes", context).hide();

  // Bugfix for attribute forms within collapsible elements. All children of the
  // collapsed fieldset are 'display: none', so jQuery takes no action if we try
  // to hide(). Showing them first forces a 'display: block' which in turn
  // forces jQuery to set 'display: none' on the element's style attribute.
  jQuery('.collapsed div.attribute', context).show();

  jQuery('.uc-conditional-attributes-parent-attr', context).each(function() {
    uc_conditional_attributes_parent_attr_trigger(context, this);
  });

  jQuery(".attributes", context).show();
}

/**
 * Binds uc_conditional_attributes_rebuild_attribute_display to the onchange event so that the display can be
 * updated as new options are selected.
 */
function uc_conditional_attributes_bind_attribute_display(context) {
  jQuery('.uc-conditional-attributes-parent-attr, .uc-conditional-attributes-parent-dependent-attr', context).change(function() {
    uc_conditional_attributes_rebuild_attribute_display(context);
  });
}

/**
 * Rebuilds the display of a parent and its dependent children.
 */
function uc_conditional_attributes_parent_attr_trigger(context, element, force_hide) {
  var selected_option = element.value;
  var node_type = jQuery('input.uc_conditional_attributes_flag', context).val();
  var this_attr_id = null;
  var matches = jQuery(element).attr("class").match(/(?:^|\s)uc-conditional-attributes-aid-(\d+)(?:\s|$)/);
  if (matches === null) {
    uc_conditional_attributes_debug('No class matches found on element #' + element.id + ': aborting.');
    return;
  }
  else {
    this_attr_id = matches[1];
  }

  var uc_ca_def = Drupal.settings.uc_conditional_attributes[node_type][this_attr_id];
  var hidden_attributes = [];

  uc_conditional_attributes_debug('[start] Type ' + node_type + ', AID ' + this_attr_id + ', OID ' + selected_option + ', definition:');
  uc_conditional_attributes_debug(uc_ca_def);

  var tracker = {};
  for (var oid in uc_ca_def) {
    for (var attr_id in uc_ca_def[oid]) {
      // a force hide may be required because we have hidden its parent
      if (force_hide === true) {
        tracker[attr_id] = false;
        uc_conditional_attributes_debug('[hide] AID ' + attr_id + ' forced');
      }
      // 'enable' relationship: attributes are hidden until enabled
      else if (uc_ca_def[oid][attr_id] == 'enable') {
        tracker[attr_id] = false;
        uc_conditional_attributes_debug('[hide] AID ' + attr_id + ' tentatively');
      }
      // 'disable' relationship: attributes are shown until they are disabled
      // however, do not show if 'please select...' is showing.
      else if (uc_ca_def[oid][attr_id] == 'disable' && selected_option != '') {
        tracker[attr_id] = true;
        uc_conditional_attributes_debug('[show] AID ' + attr_id + ' tentatively');
      }
    }
  }
  // Show ones we should be showing because of user selections
  if (selected_option in uc_ca_def) {
    for (var attr_id in uc_ca_def[selected_option]) {
      if (uc_ca_def[selected_option][attr_id] == 'enable') {
        tracker[attr_id] = true;
        uc_conditional_attributes_debug('[show] AID ' + attr_id + ' because of definition');
      }
      else if (uc_ca_def[selected_option][attr_id] == 'disable') {
        tracker[attr_id] = false;
        uc_conditional_attributes_debug('[hide] AID '  + attr_id + ' because of definition');
      }
    }
  }
  uc_conditional_attributes_debug('[status] Finished calculating attribute visiblities, taking action...');
  for (var attr_id in tracker) {
    if (tracker[attr_id]) {
      jQuery("div.attribute-" + attr_id, context).show();
    }
    else {
      jQuery("div.attribute-" + attr_id, context).hide();
      _uc_conditional_attributes_unset_hidden_attributes(context, attr_id);
      force_hide = true;
    }
    jQuery('div.attribute-' + attr_id + ' select.uc-conditional-attributes-parent-dependent-attr', context).each(function() {
      uc_conditional_attributes_debug('[recursing] AID ' + attr_id + ' with force_hide ' + force_hide);
      uc_conditional_attributes_parent_attr_trigger(context, this, force_hide);
    });
  }

  uc_conditional_attributes_debug('[done] AID ' + this_attr_id);
}

/**
 * Empties the value of a hidden attribute with attr_id. , context is the JQuery
 * context for the form.
 */
function _uc_conditional_attributes_unset_hidden_attributes(context, attr_id) {
  if (jQuery("div.attribute-" + attr_id + " input", context).is("input:radio")) {
    jQuery("div.attribute-" + attr_id + " input", context).attr("checked", false);
  }
  else {
    jQuery("div.attribute-" + attr_id + " input", context).val("");
    //jQuery("div.attribute-" + attr_id + " select", context).val("");
    jQuery("div.attribute-" + attr_id + " select option:first", context).attr('selected','selected');
  }
}
