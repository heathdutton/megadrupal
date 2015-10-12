(function($) {
  /**
   * Initialization
   */
  Drupal.behaviors.jquery_wrt = {
    /**
     * Run Drupal module JS initialization.
     * 
     * @param context
     * @param settings
     */
    attach : function(context, settings) {
      // Global context!
      var filter = "table";
      if(jQuery.trim(settings.jquery_wrt.jquery_wrt_subselector) != ''){
        // Subselector
        filter = filter + settings.jquery_wrt.jquery_wrt_subselector;
      }
      var elements = $(context).find(filter);
      
      var responsiveTable = elements.responsiveTable({
        // Get default options from drupal to make them easier accessible.
        /**
         * Keep table components classes as far as possible for the responsive
         * output.
         */
        preserveClasses : settings.jquery_wrt.jquery_wrt_preserve_classes,
        /**
         * true: Toggle table style if settings.dynamicSwitch() returns true.
         * false: Only convert to mobile (one way)
         */
        dynamic : settings.jquery_wrt.jquery_wrt_dynamic,
        /**
         * (Only used if dynamic!) If this function returns true, the responsive
         * version is shown, else displays the default table. Might be used to set
         * a switch based on orientation, screen size, ... for dynamic switching!
         * 
         * @return boolean
         */
        displayResponsiveCallback : function() {
          return $(document).width() < settings.jquery_wrt.jquery_wrt_breakpoint;
        },
        /**
         * (Only used if dynamic!) Display a link to switch back from responsive version to original table version.
         */
        showSwitch : settings.jquery_wrt.jquery_wrt_showswitch,
        /**
         * (Only used if showSwitch: true!) The title of the switch link.
         */
        switchTitle : Drupal.t('Switch to default table view'),
        
        // Selectors
        /**
         * The header columns selector.
         * Default: 'thead td, thead th';
         * other examples: 'tr th', ...
         */
        headerSelector : settings.jquery_wrt.jquery_wrt_header_selector,
        /**
         * The body rows selector.
         * Default: 'tbody tr';
         * Other examples: 'tr', ...
         */
        bodyRowSelector : settings.jquery_wrt.jquery_wrt_row_selector,
        
        // Elements
        /**
         * The responsive rows container
         * element. 
         * Default: '<dl></dl>';
         * Other examples: '<ul></ul>'.
         */
        responsiveRowElement : settings.jquery_wrt.jquery_wrt_responsive_row_element,
        /**
         * The responsive column title
         * container element.
         * Default: '<dt></dt>'; 
         * Other examples: '<li></li>'.
         */
        responsiveColumnTitleElement : settings.jquery_wrt.jquery_wrt_row_responsive_column_title_element,
        /**
         * The responsive column value container element. 
         * Default: '<dd></dd>'; 
         * Other examples: '<li></li>'.
         */
        responsiveColumnValueElement : settings.jquery_wrt.jquery_wrt_row_responsive_column_value_element
      });
      
      // Update on Window Resize! (May be buggy in some browsers, sorry.)
      if(settings.jquery_wrt.jquery_wrt_update_on_resize){
        $(window).resize(function() {
          responsiveTable.responsiveTableUpdate();
        });
      }
      
      // Attach object globally to make access easy for custom usage.
      Drupal.behaviors.jquery_wrt.responsiveTable = responsiveTable;
    }
  };
})(jQuery);