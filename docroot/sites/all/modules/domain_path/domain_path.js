(function($) {

  Drupal.behaviors.domainPath = {
    attach: function (context, settings) {
      Drupal.domainPath.init(context);
    }
  };

  /**
   * Function to check for checked state in domain access options, and only show
   * domain path fields if the domain access is enabled.
   */
  Drupal.domainPath = {
    init: function(context) {
      // Initial state for path fields.
      Drupal.domainPath.showHideAll();

      // The user doesn't have access to the domain settings so go ahead and
      // show the path field for domain that was set from the form.
      if (Drupal.settings.hasOwnProperty('domainPath') && Drupal.settings.domainPath.hasOwnProperty('domainId')) {
        Drupal.domainPath.showHide(Drupal.settings.domainPath.domainId, 'show');
      }

      // Check to see if Publish to all is checked, if so, show all fields
      $('#edit-domain-site').bind('change', function() {
        Drupal.domainPath.showHideAll();
      });

      // Bind the click event on the checkboxes and check for "checked" then
      // hide or show the domain path div accordingly.
      $('#edit-domains .form-checkbox').each(function(index) {
        $(this).bind('change', function () {
          Drupal.domainPath.showHideSelectedDomain(this);
        });
      });
    },

    /**
     * Determine the state of the "Publish to all" checkbox.
     */
    isDomainSite: function() {
      if ($('#edit-domain-site').is(':checked')) {
        return true;
      }
      else {
        return false;
      }
    },

    /**
     * Update visibility of all domain path inputs.
     */
    showHideAll: function() {
      // Hide all just incase the user doesn't have access to all domains.
      $('#edit-domain-path .form-type-textfield').css('display', 'none');
      $('#edit-domains .form-checkbox').each(function(index) {
        Drupal.domainPath.showHideSelectedDomain(this);
      });
    },

    /**
     * Show or hide the path field for the corresponding domain.
     */
    showHide: function(domainId, state) {
      var pathSelector = '.form-item-domain-path-' + domainId;

      if (state == 'show') {
        $(pathSelector).show();
      }
      else {
        $(pathSelector).hide();
      }
    },

    /**
     * Show or hide each the path field depending on its corresponding access
     * options state.
     */
    showHideSelectedDomain: function (selector) {
      var domainId = $(selector).val();
      var isDomainSite = Drupal.domainPath.isDomainSite();

      if (isDomainSite == true || $(selector).is(':checked')) {
        Drupal.domainPath.showHide(domainId, 'show');
      }
      else {
        Drupal.domainPath.showHide(domainId);
      }
    }
  }
})(jQuery);
