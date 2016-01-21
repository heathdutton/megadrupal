;(function($, w, doc){

Drupal.behaviors.something = {
	attach: function (context, settings) {

  "use strict";

  // Local object for method references
  var Nav = {};

  // Namespace
  Nav.ns = "Navigation";

  /*
    Grab the screen size psuedo element content
  */

  Nav.isScreenSize = function( sizeString ) {

    var size = w.getComputedStyle(doc.body,':before').getPropertyValue('content');

    if( size && size.indexOf( sizeString ) !==-1 ) {
      return true;
    }

  };

  /*
    Anything you need to create the small screen namvigation
  */

  Nav.createSmallView = function( parentMenu, subMenu ) {

    // don't run this code if it already ran, please
    if( parentMenu.hasClass('responsive-nav') ) {
      return;
    }

    var anchorText = parentMenu.find('a').first().text();
    var idSlug = anchorText.toLowerCase().replace(/ /g,"-");
    var randomNumber= Math.floor( Math.random()*9999 );
    var slug = idSlug + randomNumber;
    var button;
    var buttonText;
    var inverseText;

    subMenu.attr('aria-expanded', 'false').attr('id', slug);
    parentMenu.addClass('is-expandable').addClass('responsive-nav');
    parentMenu.find('a').first().after('<button type="button" aria-controls="' + slug + '" class="ui-toggle-button" data-text="close">open</button>');

    parentMenu.find('.ui-toggle-button').first().on('click', function() {

      button = $(this);
      buttonText = button.text();
      inverseText = button.attr('data-text');

      if( subMenu.attr('aria-expanded') === 'false' ) {

        subMenu.attr('aria-expanded', 'true');
        subMenu.focus();

      } else {

        subMenu.attr('aria-expanded', 'false');
        button.focus();

      }

      // toggle the button text
      button.attr('data-text', buttonText).text(inverseText);

    });

  }; // Nav.createSmallView

  /*
    Anything you need to do to remove elements that were just for small screen
  */

  Nav.destroySmallView = function( parentMenu, subMenu ) {

    subMenu.removeAttr('aria-expanded');
    parentMenu.removeClass('is-expandable');
    parentMenu.find('.ui-toggle-button').remove();
    parentMenu.removeClass('responsive-nav');

  }; // Nav.destroySmallView

  /*
    Blur event to close the menu when tabbing through
  */

  Nav.blur = function( self, parentContainer ) {

    parentContainer = self.closest('.expanded');

    if( parentContainer.find('.menu').find('ul > li').children(':focus').length === 0 ) {
      parentContainer.removeClass('child-has-focus');
    }

  }; // Nav.blur

  /*
    Focus event to expose submenus while tabbing
  */

  Nav.focus = function ( self ) {

    self.closest('.expanded').addClass('child-has-focus');

  }; // Nav.focus

  /*
    Small screen menu toggle
  */

  $('.nav-menu-toggle').on('click', function(e) {

    e.preventDefault();

    var self = $(this);
    var body = $('body');
    var targetId = self.attr('aria-controls');
    var targetZone = $('.' + targetId);

    if( body.hasClass('nav-is-active') ) {

      self.trigger('navclose');
      body.removeClass('nav-is-active');
      targetZone.attr('aria-expanded', 'false');

    } else {

      self.trigger('navopen');
      body.addClass('nav-is-active');
      targetZone.attr('aria-expanded', 'true').focus();

    }

  }); // menu toggle

  /*
    Loop over all the menu items to see if they have child nodes
  */

  $('.expanded').each(function() {

    var self = $(this);
    var menuChildren = self.children().length;

    // check to see if any menus have children
    if(menuChildren > 1) {

      self.addClass('has-children');

    }

  }); // each menu-item

  /*
    Loop through the sub menus
  */

  if( $('.expanded').length > 0 ) {

    $('.expanded').each(function() {

      var self = $(this);
      var parentMenu = self;
      var subMenu = self.find('.menu');
      var subMenuAnchor = subMenu.find('a');
      var parentContainer;

      // focus
      subMenuAnchor.on('focus', function() {
        Nav.focus( $(this) );
      });

      // blur
      subMenuAnchor.on('blur', function() {
        Nav.blur( $(this), parentContainer );
      }); // blur event

      // create small screen navigation
      if( Nav.isScreenSize( 'smallscreen' ) ) {
        Nav.createSmallView(parentMenu, subMenu);
      }

    }); // each()

    /*
      Resize event to create and destory the navigation
    */

    $(w).on('resize', function() {

      if( Nav.isScreenSize( 'smallscreen' ) ) {

        var parentMenu;
        var subMenu;

        $('.expanded').each(function() {

          parentMenu = $(this);
          subMenu = parentMenu.find('.menu');

          Nav.createSmallView( parentMenu, subMenu );

        }); // each menu with children

      } else {

        $('.expanded').each(function() {

          parentMenu = $(this);
          subMenu = parentMenu.find('.menu');

          Nav.destroySmallView( parentMenu, subMenu );

        }); // each menu with children

      }

    }); // resize

  }// if has-children

	}
};
} ( jQuery, this, this.document ));