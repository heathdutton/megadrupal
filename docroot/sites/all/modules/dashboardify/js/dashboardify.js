/**
 * @file
 * Attaches behaviors for the Dashboardify module.
 */

(function ($) {

/**
 * Implements Drupal.behaviors for the Dashboardify module.
 */
Drupal.behaviors.dashboardify = {
  attach: function (context, settings) {
    Drupal.behaviors.dashboardify.addPlaceholders();
    Drupal.behaviors.dashboardify.setupDrawer();
  },

  addPlaceholders: function() {
    $('.page-user-dashboard #content').each(function () {
      var empty_text = "";
      // If the region is empty
      if ($('.block', this).length == 0) {
        // Check if we are in customize mode and grab the correct empty text
        if ($('#dashboardify').hasClass('customize-mode')) {
          empty_text = Drupal.settings.dashboardify.emptyRegionTextActive;
        } else {
          empty_text = Drupal.settings.dashboardify.emptyRegionTextInactive;
        }
        // We need a placeholder.
        if ($('.placeholder', this).length == 0) {
          $(this).append('<div class="placeholder"></div>');
        }
        $('.placeholder', this).html(empty_text);
      }
      else {
        $('.placeholder', this).remove();
      }
    });
  },

  /**
   * Sets up the drag-and-drop behavior and the 'close' button.
   */
  setupDrawer: function () {
    // Initialize drag-and-drop.
    var region = $('.page-user-dashboard #content .item-list');
    region.sortable({
      connectWith: region,
      cursor: 'move',
      cursorAt: {top:0},
      dropOnEmpty: true,
      items: 'div.block',
      tolerance: 'pointer',
      placeholder: "ui-state-highlight",
      start: Drupal.behaviors.dashboardify.start,
      over: Drupal.behaviors.dashboardify.over,
      sort: Drupal.behaviors.dashboardify.sort,
      update: Drupal.behaviors.dashboardify.update
    })
    $(".page-user-dashboard #content .item-list .block")
      .addClass("ui-widget ui-widget-content ui-helper-clearfix ui-corner-all")
      .find("h2")
        .addClass("ui-widget-header ui-corner-all")
        .end()
      .find(".content");
  },

  /**
   * Adapts a block's position to stay connected with the mouse pointer.
   *
   * This function is called on the jQuery UI Sortable "sort" event.
   *
   * @param event
   *  The event that triggered this callback.
   * @param ui
   *  An object containing information about the item that is being dragged.
   */
  sort: function (event, ui) {
    var item = $(ui.item);

    if (event.pageX > ui.offset.left + item.width()) {
      item.css('left', event.pageX);
    }
  },

  /**
   * Sends block order to the server, and expand previously disabled blocks.
   *
   * This function is called on the jQuery UI Sortable "update" event.
   *
   * @param event
   *   The event that triggered this callback.
   * @param ui
   *   An object containing information about the item that was just dropped.
   */
  update: function (event, ui) {
    var item = $(ui.item);
    var data = [];
    $('.page-user-dashboard .dashboardify-column .item-list').each(function (index) {
      data[index] = [];
      $(this).find('.block.dashboardify').each(function (i, block) {
        itemClass = $(block).attr('class');
        bid = itemClass.match(/\bdashboardify-(\S+)\b/)[1];
        console.log(bid);
        data[index][i] = bid;
      });
    });
    $
    // Let the server know what the new block order is.
    $.get(
      Drupal.settings.dashboardify.updatePath,
      {
        'token': Drupal.settings.dashboardify.formToken,
        'col1[]': data[0],
        'col2[]': data[1],
        'col3[]': data[2],
        'col4[]': data[3],
        'col5[]': data[4],
        'col6[]': data[5],
        'col7[]': data[6],
        'col8[]': data[7],
        'col9[]': data[8],
      },
      function(data){
        message = Drupal.t('Dashboard was updated successfuly.');
        $('body')
          .append('<div class="messages status dashboardify">' + message + '</div>')
          .find('.messages.dashboardify')
          .fadeOut(2000);
      },
      'json'
    );
  }
};

})(jQuery);
