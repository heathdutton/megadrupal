(function ($) {
  Drupal.behaviors.views_quicksand_settings =  {
    attach: function(context) {
      var $select = $('#edit-style-options-easing-select');
      var $easing = $('input[name="style_options[easing]"]');

      // snippet to get easing effects from 
      // http://jqueryui.com/demos/effect/#easing
      $.each( $.easing, function( name, impl ) {
        // skip linear/jswing and any non functioning implementation
        if ( !$.isFunction( impl ) || /jswing/.test( name ) ) {
          return;
        }
        $select.append('<option value="' + name + '">' + name + '</option>');
      });
      
      // preselect the value
      $select.val($easing.val());
      
      // write the selected option to the hidden input field
      $select.change(function() {
        $easing.val($(this).val());
      });
    }
  };
})(jQuery);
