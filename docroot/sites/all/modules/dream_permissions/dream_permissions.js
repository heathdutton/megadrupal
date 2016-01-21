(function (Drupal, $) {
  'use strict';

  Drupal.behaviors.dreamPermissions = {
    attach: function (context, settings) {
      $('body').once('dream-permissions', function () {
        var $buttonFetch = $('<button>Fetch permissions</button>');
        var $buttonSave = $('<button>Save permissions</button>');
        var $spinner = $('<div class="spinner">' + Drupal.t('Please wait') + '</div>');
        var $statusMessage = $('<div class="status-message"></div>');
        var $actions = $('.dream-permissions--form .form-actions');
        var $checks = $('.dream-permissions--checks');
        var token = '';
        var originalPermissions = {};

        // Show filters.
        $('.dream-permissions--form .no-js').removeClass('no-js');

        // Append select all/none.
        $('.dream-permissions--form .dream-permissions--filter').after('<button class="button--select-all">' + Drupal.t('Select all') + '</button><button class="button--select-none">' + Drupal.t('Select none') + '</button>');

        $actions.append($buttonFetch);
        $actions.append($buttonSave);
        $actions.append($spinner);
        $actions.append($statusMessage);
        $buttonSave.hide();
        $('.dream-permissions--form #edit-submit').remove();

        $('.dream-permissions--form .dream-permissions--filter').bind('input', function (event) {
          var $filter = $(this);
          var search = $filter.val().toLowerCase();
          $filter.closest('.fieldset-wrapper').find('.form-type-checkbox').each(function () {
            if (this.innerText.toLowerCase().indexOf(search) !== -1) {
              this.classList.remove('unmatched');
            }
            else {
              this.classList.add('unmatched');
            }
          });
        });

        $('.dream-permissions--form .button--select-all').bind('click', function (event) {
          event.preventDefault();
          $(this).closest('.fieldset-wrapper').find('.form-type-checkbox:not(.unmatched) input[type="checkbox"]').each(function () {
            this.checked = true;
          });
        });

        $('.dream-permissions--form .button--select-none').bind('click', function (event) {
          event.preventDefault();
          $(this).closest('.fieldset-wrapper').find('.form-type-checkbox:not(.unmatched) input[type="checkbox"]').each(function () {
            this.checked = false;
          });
        });

        $buttonSave.bind('click', function (event) {
          event.preventDefault();
          $spinner.addClass('active');
          $statusMessage.text(Drupal.t('Saving permisisons ...'));

          var data = {};
          $checks.find('input[type="checkbox"]').map(function () {
            // Split name in role and permission.
            var parts = this.name.match(/(\d*)\[(.*)\]/);
            if (typeof data[parts[1]] === 'undefined') {
              data[parts[1]] = {};
            }
            data[parts[1]][parts[2]] = this.checked ? '1' : '0';
          });

          // Issue a post request.
          $.ajax({
            type: 'post',
            url: Drupal.settings.basePath + Drupal.settings.pathPrefix + 'admin/people/dream_permissions/save',
            data: {
              token: token,
              originalPermissions: originalPermissions,
              permissions: data
            },
            error: function (jqXHR, textStatus, errorThrown) {
              $spinner.removeClass('active');
              $statusMessage.text(Drupal.t('Something went wrong while saving ...'));
            },
            success: function (data) {
              $spinner.removeClass('active');
              $statusMessage.text(Drupal.t('Permissions saved.'));
            }
          });
        });

        $buttonFetch.bind('click', function (event) {
          event.preventDefault();
          $spinner.addClass('active');

          var selectedRoles = $('.dream-permissions--role-names input:checked').map(function () {
            return this.value;
          }).get().join(',');
          var selectedModules = $('.dream-permissions--module-names input:checked').map(function () {
            return this.value;
          }).get().join(',');

          if (selectedRoles.length > 0 && selectedModules.length > 0) {
            $statusMessage.text(Drupal.t('Fetching permisisons ...'));
            var url = Drupal.settings.basePath + Drupal.settings.pathPrefix + 'admin/people/dream_permissions/fetch/' + selectedModules + '/' + selectedRoles;
            var permission_filter = $('.dream-permissions--filter-permission').val();
            if (permission_filter) {
              url += '/' + permission_filter;
            }
            $.ajax({
              url: url,
              error: function (jqXHR, textStatus, errorThrown) {
                $spinner.removeClass('active');
                $statusMessage.text(Drupal.t('Something went wrong ...'));
              },
              success: function (data) {
                // Store the token.
                token = data.token;

                // Collapse fieldsets.
                $('.dream-permissions--form fieldset').addClass('collapsed');

                // Clean output.
                $checks.empty();

                // Add filter for the rows.
                var $filterTable = $('<input class="form-text" type="text" placeholder="' + Drupal.t('Filter table rows') + '">');
                $filterTable.bind('input', function () {
                  var $filter = $(this);
                  var search = $filter.val().toLowerCase();
                  $('#permissions-table tr:not(:first)').each(function () {
                    if (this.innerText.toLowerCase().indexOf(search) !== -1) {
                      this.classList.remove('unmatched');
                    }
                    else {
                      this.classList.add('unmatched');
                    }
                  });
                });
                $checks.append($filterTable);

                // Add button to show all rows.
                var $showAll = $('<button>' + Drupal.t('Show all rows') + '</button>');
                $showAll.bind('click', function (event) {
                  event.preventDefault();
                  $filterTable.val('');
                  $('#permissions-table tr.unmatched--check').removeClass('unmatched--check');
                });
                $checks.append($showAll);

                // Add button to show all checked rows.
                var $showAllChecked = $('<button>' + Drupal.t('Show all checked rows') + '</button>');
                $showAllChecked.bind('click', function (event) {
                  event.preventDefault();
                  $('#permissions-table tr:not(:has(input[checked]))').addClass('unmatched--check');
                  $('#permissions-table tr:has(input[checked])').addClass('matched--check').removeClass('unmatched--check');
                });
                $checks.append($showAllChecked);

                // Add button to show all unchecked rows.
                var $showAllUnchecked = $('<button>' + Drupal.t('Show all unchecked rows') + '</button>');
                $showAllUnchecked.bind('click', function (event) {
                  event.preventDefault();
                  $('#permissions-table tr:not(:has(input[checked]))').addClass('matched--check').removeClass('unmatched--check');
                  $('#permissions-table tr:has(input[checked])').addClass('unmatched--check');
                });
                $checks.append($showAllUnchecked);

                var $table = $('<table id="permissions-table"></table>');
                // Add header row.
                var roles = data.roles;
                var $row = $('<tr></tr>');
                $row.append('<th class="label">' + Drupal.t('Permission') + '</th>');
                for (var role in roles) {
                  if (roles.hasOwnProperty(role)) {
                    $row.append('<th class="label">' + roles[role] + '</th>');

                  }
                }
                $table.append($row);

                // Add checkbox rows.
                var permissions = data.permissions;
                originalPermissions = permissions;
                var permissionsNames = data.permissionsNames;

                for (var permissionName in permissions['2']) {
                  if (permissions['2'].hasOwnProperty(permissionName)) {
                    $row = $('<tr></tr>');
                    $row.append('<td class="label">' + permissionsNames[permissionName] + '</td>');
                    for (var rid in permissions) {
                      if (permissions.hasOwnProperty(rid)) {
                        // Do not disable for anonymous and authenticated user.
                        var disabled = rid !== '2' && rid !== '1' && permissions['2'][permissionName] === '1' ? ' disabled="disabled"' : '';
                        if (permissions[rid][permissionName] === '1') {
                          $row.append('<td class="item"><input type="checkbox" name="' + rid + '[' + permissionName + ']" value="1" checked="checked"' + disabled + '></td>');
                        }
                        else {
                          $row.append('<td class="item"><input type="checkbox" name="' + rid + '[' + permissionName + ']" value="1"' + disabled + '></td>');
                        }
                      }
                    }
                    $table.append($row);
                  }
                }

                $table.find('input[name^="2["]').change(function () {
                  var name = $(this).attr('name');
                  name = '[' + name.replace(/^\d+\[/, '');
                  if ($(this).is(':checked')) {
                    $('#permissions-table').find('input[name$="' + name + '"]')
                      .not(this)
                      .not('input[name="1' + name + '"]')
                      .attr('disabled', 'disabled');
                  }
                  else {
                    $('#permissions-table').find('input[name$="' + name + '"]')
                      .not(this)
                      .not('input[name="1' + name + '"]')
                      .removeAttr('disabled');
                  }
                });

                $checks.append($table);
                $buttonSave.show();
                $spinner.removeClass('active');
                $statusMessage.text('');
              }
            });
          }
          else {
            $spinner.removeClass('active');
            $statusMessage.text(Drupal.t('Please select at least one role and module.'));
          }
        });
      });
    }
  };
})(Drupal, jQuery);
