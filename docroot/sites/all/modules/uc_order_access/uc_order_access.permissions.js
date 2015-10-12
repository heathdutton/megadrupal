(function ($) {
/**
 * Shows checked and disabled checkboxes for inherited permissions.
 *
 * @todo - This is my first jQuery script so maybe it needs a little tuning.
 */
Drupal.behaviors.orderPermissions = {
  attach: function (context) {
    var self = this;
    $('table#order-permissions').once('orderPermissions', function () {
      // On a site with many roles and permissions, this behavior initially has
      // to perform thousands of DOM manipulations to inject checkboxes and hide
      // them. By detaching the table from the DOM, all operations can be
      // performed without triggering internal layout and re-rendering processes
      // in the browser.
      var $table = $(this);
      if ($table.prev().length) {
        var $ancestor = $table.prev(), method = 'after';
      }
      else {
        var $ancestor = $table.parent(), method = 'append';
      }
      $table.detach();

      // Create dummy checkboxes. We use dummy checkboxes instead of reusing
      // the existing checkboxes here because new checkboxes don't alter the
      // submitted form. If we'd automatically check existing checkboxes, the
      // permission table would be polluted with redundant entries. This
      // is deliberate, but desirable when we automatically check them.
      var $dummy = $('<input type="checkbox" class="dummy-checkbox" disabled="disabled" checked="checked" />')
        .hide();

      $('input[type=checkbox]', this).each(function () {
        $dummy.clone()
        .attr('id', $(this).attr('id') + '-dummy')
        .addClass($(this).attr('class'))
        .insertAfter(this);
      }).addClass('real-checkbox');

      // Initialize the authenticated user checkbox.
      $('input[type=checkbox].rid-2', this).not('.dummy-checkbox')
        .bind('click.permissions', self.toggleAuthCheckbox)
        // .triggerHandler() cannot be used here, as it only affects the first
        // element.
        .each(self.toggleAuthCheckbox);

      // Re-insert the table into the DOM.
      $ancestor[method]($table);
    });

    // This table need to be processed here, after dummy checkboxes are set in
    // order for dummy checkboxes to be displayed for selected standard order
    // permission on first load of the form page.
    $('table#permissions').once('orderPermissions', function () {
      var $table = $(this);
      if ($table.prev().length) {
        var $ancestor = $table.prev(), method = 'after';
      }
      else {
        var $ancestor = $table.parent(), method = 'append';
      }
      $table.detach();

      // Because our permission have diffrent names than standard order permission
      // we need to map standard permission to our permission.
      var permmissionMap = {
        'view all orders': 'view-order',
        'edit orders': 'edit-order',
        'delete orders': 'delete-order',
        'unconditionally delete orders': 'delete-order',
      };

      // Set a var here to traverse the DOM once.
      var realCheckboxes = $('input[type=checkbox]', this).not('.dummy-checkbox');
      // Add our permission name as class to corresponding standard permission.
      realCheckboxes.each(function(){
        // Find standard permission rid and permission.
        var nameComponents = $(this).attr('name').split('][');
        var rid = nameComponents[1];
        var permission = nameComponents[2].slice(0,-1);
        // Add a class with our permission name.
        if(permission in permmissionMap) {
          $(this).addClass(permmissionMap[permission]);
        }
      });

      // Initialize the standard order permission checkbox.
      realCheckboxes
        .bind('click.permissions', self.toggleStandardOrderPermission)
        .each(self.toggleStandardOrderPermission);

      // Re-insert the table into the DOM.
      $ancestor[method]($table);
    });
  },

  toggleStandardOrderPermission: function() {
    var self = Drupal.behaviors.orderPermissions;
    // Checked standard permission object.
    var permOverwrittenCheckbox = this;
    // Find checked standard permission rid and permission
    var nameComponents = $(this).attr('name').split('][');
    var rid = nameComponents[1];
    var permission = nameComponents[2].slice(0,-1);

    // Because our permission have diffrent names than standard order permission
    // we need to map standard permission to our permission.
    var permmissionMap = {
      'view all orders': 'view-order',
      'edit orders': 'edit-order',
      'delete orders': 'delete-order',
      'unconditionally delete orders': 'delete-order',
    };

    // Overwrite every row of every order status in our permission table.
    $('input[type=checkbox].rid-' + rid + '.' + permmissionMap[permission], $('table#order-permissions'))
    .not('.dummy-checkbox').each(function() {
      var $row = $(this).closest('tr'), authCheckbox;
      // Find displayed auth checkbox dummy or real and set it as authcheckbox var.
      $('input[type=checkbox].rid-2.real-checkbox', $row).each(function(){
          authCheckbox = this;
      });
      // We only take further action if authenticated checkbox on each row is
      // not chekced or role is anonymous or authenticated.
      if(!authCheckbox.checked || rid == "1" || rid == "2") {
        // Only react on visible checkboxes.
        if(permOverwrittenCheckbox.style.display == "") {
          // Hide / show real checkbox.
          this.style.display = (permOverwrittenCheckbox.checked ? 'none' : '');
          var realCheckbox = this;

          // Find the dummy checkbox.
          $(this).closest('div').find('.dummy-checkbox').each(function(){
            // If permission is overwritted set the title and display the dummy.
            if(permOverwrittenCheckbox.checked) {
              $(this).attr('title', Drupal.t("This permission is overwritten by standard order permission: " + permission));
            }
            this.style.display = (permOverwrittenCheckbox.checked ? '' : 'none');

            // If dummy has rid 2 (authenticated user) display the dummy checkboxes
            // for authenticated role.
            if($(this).hasClass('rid-2')) {
              $(this).each(self.toggleAuthCheckbox);
            }
          });
        }
      }
    });
  },

  toggleAuthCheckbox: function() {
    var $row = $(this).closest('tr'), authCheckbox;

    // Find displayed auth checkbox dummy or real and set it as authcheckbox var.
    $('input[type=checkbox].rid-2', $row).each(function(){
      if(this.style.display == ""){
        authCheckbox = this;
      }
    });

    // Find realcheckboxes on selected row except anonymous and authenticated.
    $row.find('.real-checkbox').not('.rid-2, .rid-1').each(function () {
      // Find rid and permission for each checkbox to identify the standard
      // permission checkbox in standard permissions table.
      var nameComponents = $(this).attr('name').split('][');
      var rid = nameComponents[1];
      var permission = nameComponents[2].slice(0,-1).split(' ').join('-');

      // Find out if one of our permissions is overwritted by a standard
      // permission to display the dummy for it if so.
      var overwritten = false, stdPermName;
      $('input[type=checkbox].rid-' + rid + '.' + permission, $('table#permissions')).not('.dummy-checkbox').each(function(){
        if(this.style.display == "" && this.checked) {
          overwritten = true;
          var stdPermNameComponents = $(this).attr('name').split('][');
          stdPermName = stdPermNameComponents[2].slice(0,-1);
        }
      });
      this.style.display = (authCheckbox.checked || overwritten ? 'none' : '');

      // Find the dummy for this checkbox.
      $(this).closest('div').find('.dummy-checkbox').each(function () {
        // If authenticated checkbox is checked display this to the user.
        if(authCheckbox.checked) {
          $(this).attr('title', Drupal.t("This permission is inherited from the authenticated user role."));
        }
        else if(overwritten) {
          $(this).attr('title', Drupal.t("This permission is overwritten by standard order permission: " + stdPermName));
        }
        this.style.display = (authCheckbox.checked || overwritten ? '' : 'none');
      });
    });
  }
};
})(jQuery);
