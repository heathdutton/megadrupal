(function ($) {
  function setCookie(cname, cvalue, exdays) {
      var d = new Date();
      d.setTime(d.getTime() + (exdays*24*60*60*1000));
      var expires = "expires="+d.toUTCString();
      document.cookie = cname + "=" + cvalue + ";path=/;" + expires;
  }

  function getCookie(cname) {
      var name = cname + "=";
      var ca = document.cookie.split(';');
      for(var i=0; i<ca.length; i++) {
          var c = ca[i];
          while (c.charAt(0)==' ') c = c.substring(1);
          if (c.indexOf(name) == 0) return c.substring(name.length, c.length);
      }
      return "";
  }

  function initToggleCookie() {
      var wrapper = $('#wrapper');
      var status = getCookie("Drupal.pmKickstartTheme.sidebar.toggle");
      if (status != "") {
        wrapper.addClass('toggled');
        setCookie("Drupal.pmKickstartTheme.sidebar.toggle", 'toggled', 365);
      }
      else {
        wrapper.removeClass('toggled');
      }
      setTimeout(wrapperCouldBeAnimated, 2000);
      function wrapperCouldBeAnimated() {
         wrapper.addClass('css-animate');
      }
  }

  jQuery(document).ready(function($) {
    initToggleCookie();
  });

  Drupal.behaviors.pmDash = {
    attach: function (context, settings) {
      $('#sidebar-toggle-button', context).click(function (e) {
        e.preventDefault();
        var wrapper = $('#wrapper');
        // $('#wrapper').toggleClass('toggled');
        if (wrapper.hasClass('toggled')) {
          wrapper.removeClass('toggled');
          setCookie("Drupal.pmKickstartTheme.sidebar.toggle", '', 365);
        }
        else {
          wrapper.addClass('toggled');
          setCookie("Drupal.pmKickstartTheme.sidebar.toggle", 'toggled', 365);
        }

       });
    }
  };
})(jQuery);


/**
 * The input field items that add displays must be rendered as <input> elements.
 * The following behavior detaches the <input> elements from the DOM, wraps them
 * in an unordered list, then appends them to the list of tabs.
 */
delete Drupal.behaviors.viewsUiRenderAddViewButton;

Drupal.behaviors.viewsUiRenderAddViewButton = {};

Drupal.behaviors.viewsUiRenderAddViewButton.attach = function (context, settings) {
  var $ = jQuery;

  // Build the add display menu and pull the display input buttons into it.
  var $menu = $('#views-display-menu-tabs', context).once('views-ui-render-add-view-button-processed');

  if (!$menu.length) {
    return;
  }
  //var $addDisplayDropdown = $('<li class="add"><a href="#"><span class="icon add"></span>' + Drupal.t('CREATE (a55)PLACEHOLDER') + '</a><ul class="action-list" style="display:none;"></ul></li>');
  var $addDisplayDropdown = $('<li class="btn-group btn-group-xs"><button type="button" class="btn btn-success dropdown-toggle" data-toggle="dropdown"><i class="fa fa-plus"></i> ' + Drupal.t('ADD') + ' <span class="caret"></span></button><ul class="dropdown-menu" role="menu"></ul></li>');
  var $displayButtons = $menu.nextAll('.add-display').detach();
  $displayButtons.appendTo($addDisplayDropdown.find('.dropdown-menu')).wrap('<li>')
    .parent().first().addClass('first').end().last().addClass('last');
  $displayButtons.each(function () {
    $(this)
      .addClass('btn-sm')
      .addClass('btn-default')
      .addClass('btn-block');
  });
  $addDisplayDropdown.appendTo($menu);
};

Drupal.behaviors.pmkickstartthemeDropdown = {};

Drupal.behaviors.pmkickstartthemeDropdown.attach = function (context, settings) {
  var $ = jQuery;
  // Build the add display menu and pull the display input buttons into it.
  var $wrapper = $('#edit-display-settings-top', context).once('pmkickstarttheme-dropdown-processed');

  var $menu = $('#edit-display-settings-top .right.actions', context).once('pmkickstarttheme-dropdown-processed');


  if (!$menu.length) {
    return;
  }
  var $displayTitle = $('<button type="button" class="btn">' + Drupal.t('Actions') +'</button>');
  if ($menu.find('li a').length > 0) {
    var $displayTitle = $menu.find('li a:first').detach();
    $displayTitle.addClass('btn btn-xs btn-info');
  };

  //var $addDisplayDropdown = $('<li class="add"><a href="#"><span class="icon add"></span>' + Drupal.t('CREATE (a55)PLACEHOLDER') + '</a><ul class="action-list" style="display:none;"></ul></li>');
  var $addDisplayDropdown = $('<div class="btn-group btn-group-xs pmkickstarttheme-button-processed"><button type="button" class="btn btn-normal dropdown-toggle" data-toggle="dropdown"> <span class="caret"></span></button><ul class="dropdown-menu btn-group-xs" role="menu"></ul></div>');
  $addDisplayDropdown.prepend($displayTitle);
  var $displayButtons = $menu.find('button').detach();
  console.log($displayButtons);
  $displayButtons.appendTo($addDisplayDropdown.find('.dropdown-menu')).parent().first().addClass('first').end().last().addClass('last');
  $displayButtons.each(function () {
    $(this)
      .addClass('btn-xs')
      .addClass('btn-block');

  });
  $wrapper.find('.ctools-dropbutton').remove();
  $addDisplayDropdown.appendTo($wrapper);
  $('#views-display-top > div.btn-group').addClass('pull-right');
  $('#views-display-menu-tabs').addClass('pull-left');
};
