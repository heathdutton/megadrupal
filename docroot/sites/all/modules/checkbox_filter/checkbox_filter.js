
Drupal.CheckboxFilter = Drupal.CheckboxFilter || {};

/**
 * Filters checkboxes based on their label
 */
Drupal.CheckboxFilter.filter = function() {
  var field = jQuery(this)[0];
  var checkboxes = jQuery(".form-checkboxes .form-item", jQuery(this).parent().parent()); 
  var found = false;
  var label = "";
  var option = null;
  for (var i = 0; i < checkboxes.length; i++) {
    option = checkboxes.eq(i);
    label = Drupal.CheckboxFilter.trim(option.text());
    if (label.toUpperCase().indexOf(field.value.toUpperCase()) < 0) {          
      option.hide();
    } else {
      option.show(); 
    }
  }
}

/**
 * Trims whitespace from strings
 */
Drupal.CheckboxFilter.trim = function(str) {
	var	str = str.replace(/^\s\s*/, ''),
		ws = /\s/,
		i = str.length;
	while (ws.test(str.charAt(--i)));
	return str.slice(0, i + 1);
}


/**
 * Finds clusters of checkboxes
 * and attach a filtering textfield to them.
 */
Drupal.behaviors.checkboxfilter = {
  attach: function (context) {
    jQuery('div.form-checkboxes:has(.form-item)', context).before('<div class="form-item"><label>' + Drupal.t('Filter') + ':</label> <input class="checkbox-filter form-text" type="text" size="8" /></div>');
    jQuery("input.checkbox-filter").bind('keyup', Drupal.CheckboxFilter.filter); 
  }
}