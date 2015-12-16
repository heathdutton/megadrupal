
/*
 * @file
 * Javascript file. Allows users to locally rearrange and roll up panes on the
 * panel and save state per account
 */

(function ($) {
  Drupal.behaviors.panelsDashboard = {

    /**
     * Snapshots current dashboard state to JSON
     */
    toJSON: function() {
      // Prepare to grab dashboard data
      var dashboardData = {};

      // Only trigger on a processed dashboard.
      $('.dashboard-display-processed').each(function() {
        var dashboard = {};
        // Cycle through the dashboard's regions.
        $('panels-dashboard-region').each(function() {
          var panes = {};
          // Cycle through each of the region's pane.
          $('#' + $(this).attr('id') + ' .panels-dashboard-pane').each(function() {
            var dashboardRolled = $(this).hasClass('dashboard-rolled') ? 1 : 0;
            panes.push('["' + $(this).attr('id') + '", ' + dashboardRolled + '"]');
          });
          dashboard.push('"' + $(this).attr('id') + '": [' + panes.join(',') + ']');
        });
        dashboardData['panels_dashboard_data'] = '{' + dashboard.join(',') + '}';
      });

      return dashboardData;
    },

    /**
     * Slides the pane up and sets correct class on it.
     */
    slideUp: function(pane, speed){
      pane.addClass("dashboard-rolled");
      pane.children(":not(.dashboard-pane-handle)").slideUp(speed);
    },

    /**
     * Slides the pane down and removes class from it.
     */
    slideDown: function(pane, speed) {
      pane.removeClass("dashboard-rolled");
      pane.children(":not(.dashboard-pane-handle)").slideDown(speed);
    },

    /**
     * Updates dashboard after some actions
     */
    refresh: function() {
      $('.dashboard-display-processed').each(function() {
        // Sets or removes dashboard-region-empty classes based on whether the
        // region is empty or not.
        // @todo: simplify logic.
        $('.panels-dashboard-region:not(:has(.panels-dashboard-pane))').addClass('panels-dashion-region-empty');
        $('.panels-dashboard-region:has(.panels-dashboard-pane)').removeClass('panels-dashion-region-empty');
      });
    },

    /**
     * Savesthe dashboard's setup.
     */
    save: function(data) {
      $.ajax({
        url: "/js/panels-dashboard/set/" + Drupal.settings.panelsDashboard.did,
        type: "POST",
        dataType: "json",
        data: data
      });
    },

    /**
     * Restores dashboard's setup.
     */
    restore: function() {
      $('.dashboard-display-processed').each(function() {
        $.ajax({
          url: '/js/panels-dashboard/get/' + Drupal.settings.panelsDashboard.did,
          dataType: 'json',
          success: function(response) {
            if (response.status == 'success' && response.data) {
              var data = response.data,
                slot, pane;
              for (slot in data) {
                for (pane in data[slot]) {
                  // Restore pane to desired location.
                  $('#' + data[slot][pane][0]).appendTo('#' + slot);

                  // Restore pane state (rolled or not.)
                  if (data[slot][pane][1]) {
                    // invoke slide up.
                    Drupal.behaviors.panelsDashboard.slideUp($('#' + data[slot][pane][0]), 1);
                  }
                }
              }
            }
            Drupal.behaviors.panelsDashboard.refresh();
            $('.dashboard-display-processed').show();
          }
        });
      });
    },

    /**
     * The attach
     */
    attach: function(context, settings) {
      $(context).find('.panel-display').once('panel-display', function() {
        // Hide while rendering
        // @todo: can we load the JSON and render from server initially?
        //$(this).hide();

        // Remove elements that are not required or simply break the whole thing.
        // @todo: is this needed?
        $('.panel-region-separator').remove();
        $('.admin-links').remove();

        // Add handles to panes
        $(this).find('.panels-dashboard-pane').each(function() {
          if ($(this).children('.pane-title').length > 0) {
            $('.pane-title').addClass('dashboard-pane-handle');
          }
          else {
            $(this).prepend('<div class="dashboard-pane-handle"></div>');
          }
        });

        // Add roll-up, roll-down icons to each pane.
        $('.dashboard-pane-handle').prepend('<div class="dashboard-roll-button"></div>');

        // Restore setup.
        Drupal.behaviors.panelsDashboard.restore();

        $('.dashboard-roll-button').click(function() {
          if ($(this).parents('.panels-dashboard-pane').hasClass('.dashboard-rolled')) {
            Drupal.behaviors.panelsDashboard.slideDown($(this).parents('.panels-dashboard-pane'), 200);
          }
          else {
            Drupal.behaviors.panelsDashboard.slideUp($(this).parents('.panels-dashboard-pane'), 200);
          }

          Drupal.behaviors.panelsDashboard.toJSON();
        });

        // Bind sortables.
        $('.panels-dashboard-region:not(.dashboard-region-processed)').addClass('dashboard-region-processed').each(function() {
          $(this).sortable({
            placeholder: 'dashboard-placeholder-highlight',
            cursor: 'move',
            tolerance: 'pointer',
            handle: '.dashboard-pane-handle',
            revert: 200,
            delay: 100,
            start: function(event, ui) {
              // If region contains only one pane, please set dashboard-region-empty class already.
              if ($(this).children('.panels-dashboard-pane').length == 1) {
                $(this).addClass("dashboard-region-empty");
              }
            },
            stop: function(event, ui) {
              Drupal.behaviors.panelsDashboard.refresh();
              Drupal.behaviors.panelsDashboard.toJSON();
            }
          });
        });
        $('.dashboard-region-processed').sortable('option', 'connectWith', ['.panels-dashboard-region']);
      });
    }
  }

})(jQuery);
