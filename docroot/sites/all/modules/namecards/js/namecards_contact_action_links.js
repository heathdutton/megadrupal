/**
 * @file
 * 
 * Javascript document
 * 
 * Create a compact layout for contact action links details, edit, and delete. 
 * This script essentially creating an alternative UI element for the default 
 * action links produced by the relevant view.
 */
(function ($) {

// Compact Link Object
var CompactLinkObj = function(element) {
  var $element = $(element);
  var $detailsLink = $('a.namecards-details-link', $element);
  var $editLink = $('a.namecards-edit-link', $element);
  var $deleteLink = $('a.namecards-delete-link', $element);
  
  this.detailsLink = null;
  this.editLink = null;
  this.deleteLink = null;

  // Extract existing link properties.
  if ($detailsLink.length) {
    this.detailsLink = {
      cssClass: $detailsLink.attr('class'), 
      url: $detailsLink.attr('href')
    };
  }
  // Extract existing link properties.
  if ($editLink.length) {
    this.editLink = {
      cssClass: $editLink.attr('class'), 
      url: $editLink.attr('href')
    };
  }
  // Extract existing link properties.
  if ($deleteLink.length) {
    this.deleteLink = {
      cssClass: $deleteLink.attr('class'), 
      url: $deleteLink.attr('href')
    };
  }
};

// Attach render method to flyout object.
CompactLinkObj.prototype.render = function() {
  var $widget = $('<div class="namecards-compact-link-wrapper"><div class="namecards-compact-link-default"></div><div class="namecards-compact-link-selector"></div><div class="namecards-compact-link-menu"><ul class="namecards-compact-link-list"></ul></div></div>');
  var $wrapper = $('.namecards-compact-link-wrapper', $widget);
  var $defaultButtonContainer = $('.namecards-compact-link-default', $widget);
  var $selector = $('.namecards-compact-link-selector', $widget);
  var $menu = $('.namecards-compact-link-menu', $widget);
  var $linkList = $('ul.namecards-compact-link-list', $widget);
  var optionsExist = false; // Refers to whether edit and/or delete options exist for this contact.
  
  // Create a temp mount point for widget in the DOM. Required
  // to access various properties (e.g. innerHeight, innerWidth, etc.)
  // of widget elements.
  if ($('#tempMount').length == 0) {
    $('body').append('<div id="tempMount"></div>')
    // Hide mount point, as should not be visible to user.
    $('#tempMount').hide();
  }
  $tempMount = $('#tempMount');
  $tempMount.append($widget);
  
  if (this.detailsLink != null) {
    $defaultButtonContainer.append($('<a href="' + this.detailsLink.url + '" class="' + this.detailsLink.cssClass + '">Details</a>'));
    
    // Center default link.
    var $detailsLink = $('.namecards-details-link', $widget);
    var defaultHeight = $defaultButtonContainer.innerHeight();
    var defaultWidth = $defaultButtonContainer.innerWidth();
    var detailsLinkHeight = $detailsLink.outerHeight();
    var detailsLinkWidth = $detailsLink.outerWidth();
    
    // Bind hover events.
    $defaultButtonContainer.hover(
      function(event) {
        $(this).addClass('namecards-compact-link-mouse-over');
      },
      function(event) {
        $(this).removeClass('namecards-compact-link-mouse-over');
      }
    );
    $selector.hover(
      function(event) {
        $(this).addClass('namecards-compact-link-mouse-over');
      },
      function(event) {
        $(this).removeClass('namecards-compact-link-mouse-over');
      }
    );
  }
  if (this.editLink != null) {
    // An edit link exists for this contact so add an edit link to list of options in flyout.
    optionsExist = true;
    $linkList.append($('<li><a href="' + this.editLink.url + '" class="' + this.editLink.cssClass + '">Edit</a><li>'));
  }
  if (this.deleteLink != null) {
    // An delete link exists for this contact so add an delete link to list of options in flyout.
    optionsExist = true;
    $linkList.append($('<li><a href="' + this.deleteLink.url + '" class="' + this.deleteLink.cssClass + '">Delete</a><li>'));
  }
  
  // Check if edit or delete options available.
  if (optionsExist == true) {
    // Hide options flyout.
    $menu.hide();
    
    // Show or hide options menu when click selector element.
    $selector.click(function(event) {
      event.stopPropagation();
      if ($menu.is(':hidden') == true) {
        // Hide any already open menus.
        $('.namecards-compact-link-menu').filter(':visible').hide();
        // Display current menu.
        $menu.slideDown();
      }
      else {
        // Menu is already visible, so hide it.
        $menu.slideUp();
      }
    });
    // Close menu when click outside of select element.
    $('html').click(function(event) {
      $menu.slideUp();
    });
  }
  else {
    // Remove selector and options menu.
    $selector.remove();
    $menu.remove();
    // Add rounded corners to right side of default element.
    $defaultButtonContainer.addClass('namecards-compact-link-default-no-options');
  }
  
  // Remove temp mount point.
  //$('#tempMount').remove();

  return $widget;
};
  
Drupal.behaviors.NamecardsContactActionLinks = {
  attach: function(context) {
    // Add compact link to view browse_contacts.
    $('.view-content table tbody tr', '.view-id-namecards_browse_contacts').not('.namecards-compact-link-processed').addClass('namecards-compact-link-processed').each(function(index, ele) {
      var $ele = $(ele);
      // Hide table cells containing regular node links.
      $header = $ele.closest('table').find('th');
      $header.eq(-1).css({'display': 'none'});
      $header.eq(-2).css({'display': 'none'});
      $ele.children('td').eq(-1).css({'display': 'none'});
      $ele.children('td').eq(-2).css({'display': 'none'});
      $ele.children('td').eq(-3).css({'display': 'none'});
      
      // Create compact node links and add to table.
      var compactLink = new CompactLinkObj($ele);
      $ele.append($('<td class="namecards-compact-link-table-cell"></td>'));
      $ele.children('td.namecards-compact-link-table-cell').append(compactLink.render());
    });
    // Add compact link to view contacts_by_org and contacts_by_event.
    $('.view-id-namecards_contacts_by_org .view-content table tbody tr, .view-id-namecards_contacts_by_event .view-content table tbody tr').not('.namecards-compact-link-processed').addClass('namecards-compact-link-processed').each(function(index, ele) {
      var $ele = $(ele);
      // Hide table cells containing regular node links.
      $header = $ele.closest('table').find('th');
      $header.eq(-1).css({'display': 'none'});
      $header.eq(-2).css({'display': 'none'});
      $ele.children('td').eq(-1).css({'display': 'none'});
      $ele.children('td').eq(-2).css({'display': 'none'});
      $ele.children('td').eq(-3).css({'display': 'none'});
      
      // Create compact node links and add to table.
      var compactLink = new CompactLinkObj($ele);
      $ele.append($('<td class="namecards-compact-link-table-cell"></td>'));
      $ele.children('td.namecards-compact-link-table-cell').append(compactLink.render());
    });
  }
};  
    
}(jQuery));
