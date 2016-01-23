(function ($) {

  /**
   * Hijack Dashboard module js function to set up the drawer
   */
  Drupal.behaviors.dashboard.setupDrawer = function () {
    $('div.customize .canvas-content input').click(Drupal.behaviors.dashboard.exitCustomizeMode);
    $('div.customize .canvas-content')
      .append('<div class="dashboard-done-button ui-button ui-widget ui-state-default ui-corner-all ui-button-text-only">'
      + '<a class="button ui-button-text" href="' 
      + Drupal.settings.dashboard.dashboard 
      + '">' 
      + Drupal.t('Done') 
      + '</a></div>');

    // Initialize drag-and-drop.
    var regions = $('#dashboard div.region');
    regions.sortable({
      handle: 'h3',
      connectWith: regions,
      cursor: 'move',
      cursorAt: {top:0},
      dropOnEmpty: true,
      items: '> div.block, > div.disabled-block',
      placeholder: 'block-placeholder ui-state-highlight ui-corner-all clearfix',
      tolerance: 'pointer',
      start: Drupal.behaviors.dashboard.start,
      over: Drupal.behaviors.dashboard.over,
      sort: Drupal.behaviors.dashboard.sort,
      update: Drupal.behaviors.dashboard.update
    });
    
    // Add hover state
    $('.ui-button').once('ui-button').Fhover();
    $('.disabled-block h3').Fhover();
  };

  /**
   * Hijack Dashboard module js function when updating the content
   */
  Drupal.behaviors.dashboard.update = function (event, ui) {
    $('#dashboard').addClass('customize-inactive');
    var item = $(ui.item);

    // If the user dragged a disabled block, load the block contents.
    if (item.hasClass('disabled-block')) {
      var module, delta, itemClass;
      itemClass = item.attr('class');
      // Determine the block module and delta.
      module = itemClass.match(/\bmodule-(\S+)\b/)[1];
      delta = itemClass.match(/\bdelta-(\S+)\b/)[1];

      // Load the newly enabled block's content.
      $.get(Drupal.settings.dashboard.blockContent + '/' + module + '/' + delta, {},
        function (block) {
          if (block) {
            var content = $(block).find('div.block-content').html();
            item.find('div.block-content').html(content);
          }

          if (item.find('div.block-content').is(':empty')) {
            item.find('div.block-content').html(Drupal.settings.dashboard.emptyBlockText);
          }

          Drupal.attachBehaviors(item);
        },
        'html'
      );
      
      // Remove the "disabled-block" class, so we don't reload its content the
      // next time it's dragged.
      item.removeClass("disabled-block");    
    }
           
    if (item.closest('#disabled-blocks')) {
      item
        .find('h3')
        .addClass('ui-state-default ui-corner-all ui-accordion-header ui-helper-reset')
        .prepend('<span class="ui-icon ui-icon-circle-arrow-e" />')
        .unbind('mouseenter mouseleave')
        .Fhover();
    }

    // Let the server know what the new block order is.
    $.post(Drupal.settings.dashboard.updatePath, {
        'form_token': Drupal.settings.dashboard.formToken,
        'regions': Drupal.behaviors.dashboard.getOrder
      }
    );
    
    Drupal.behaviors.jqueryUIDashboardAccordionInit.attach();
    Drupal.behaviors.dashboard.addPlaceholders();
  };
  
  /**
   * Plugin for changing class according
   * to jQueryUI specification
   */
  $.fn.Fhover = function (options) {
    var defaults = {
        hoverClass: 'ui-state-hover'
    };
    
    options = $.extend(defaults, options);
    
    return this.each(function() {
      $(this).hover(function() {
        $(this)
        .toggleClass(options.hoverClass);
      }, function() {
        $(this)
        .toggleClass(options.hoverClass);
      });
    });
  };
    
  /**
 * Main file to initialize all jQuery UI Misc elements
 */
  Drupal.behaviors.jqueryUIDashboardAccordionInit = { 
    attach: function() {
      var target = $('.ui-accordion'),
          icons = {
            header: 'ui-icon-circle-arrow-e',
            headerSelected: 'ui-icon-circle-arrow-s'
        };
      target.not('.disabled-blocks').each(function() {
        var self = $(this);
        // Nuke and clean the classes and icon
        // This is needed because we want to have
        // a default state if user don't have js
        self.find('h3')
            .removeClass('ui-accordion-header ui-state-default ui-state-active ui-helper-reset')
            .children('span.ui-icon')
            .remove();
        
        // Build accordion
        self.accordion('destroy').accordion({
          header: "> div.block > h3",
          autoHeight: false,
          icons: icons
        });
          
        self.find('h3').each(function() {
          if ($(this).hasClass('ui-state-active') && $(this).hasClass('ui-corner-top') == false) {
            $(this).addClass('ui-corner-top');
          }
        });
      });

   }
  };
})(jQuery);