
var Navigate = Navigate || {};

Navigate.loaderCount = 0;

Drupal.behaviors.navigate = {
  attach: function () {

  Navigate.bindToggleButtons();
  Navigate.bindCallbackButtons();
  Navigate.bindTextInputs();
  Navigate.bindAdminTools();
  Navigate.bindTextareas();
  Navigate.bindAddWidgetList();
  
  // Run one-time bindings on Navigate area
  if (!jQuery('.navigate').hasClass('navigate-processed')) {
    jQuery('.navigate').addClass('navigate-processed')
    Navigate.bindNavigateOneTime();
    // as jquery.hotkeys.js is conflicting with d7 so disabled the code for now.
    /*jQuery('body').bind('keydown', {combi:'ctrl+shift+n'}, Navigate.toggle);
    if (jQuery('.navigate-key-button').hasClass('key-disabled')) {
      jQuery('body').unbind('keydown', {combi:'ctrl+shift+n'});
    }*/
  }


  // Bind event to widget settings button
  jQuery('.navigate-widget-settings-button').each(function () {
    if (jQuery(this).hasClass('widget-settings-button-processed')) {
      return;
    }
    jQuery(this).addClass('widget-settings-button-processed');
    jQuery(this).click(function() {
      jQuery(this).parent().find('.navigate-widget-settings-outer').slideToggle('fast');
    });
  });
  
  
  // Add select all on text inputs
  jQuery('.navigate-select-all').each(function () {
    if (jQuery(this).hasClass('select-all-processed')) {
      return;
    }
    jQuery(this).addClass('select-all-processed');
    jQuery(".navigate-select-all").focus(function () {
      jQuery(this).select();
    });
  });


  // Add tooltips - v.2
  jQuery('.navigate-tooltip').each(function () {
    if (jQuery(this).hasClass('tooltip-processed')) {
      return;
    }
    jQuery(this).addClass('tooltip-processed');
    $content = jQuery('#' + jQuery(this).attr('id') + '_tip_content').html();
    if ($content != '') {
      jQuery(this).tooltip({
        bodyHandler: function() {
          return jQuery('#' + jQuery(this).attr('id') + '_tip_content').html();
        },
        track: true,
        fixPNG: true,
        fade: 150,
        delay:750
      });
    }
  });

  // Bind widget close button
  jQuery('.navigate-widget-close').click(function() {
    if (jQuery(this).hasClass('widget-close-processed')) {
      return;
    }
    jQuery(this).addClass('widget-close-processed');
    var wid = jQuery(this).find('.navigate-widget-close-id').val();
    
    Navigate.queueAdd();
    jQuery.ajax({
        url: Navigate.ajaxUrl(),
        type: 'POST',
        data: 'action=widget_delete&wid=' + wid,
        error: function() {
        },
        success: function(msg) {
          jQuery('#navigate-widget-outer-' + wid).slideUp('fast', function() {
            jQuery('#navigate-widget-outer-' + wid).remove();
            Navigate.queueSubtract();
          });
        }
      });
  });
  
  
  // Add double-click to widget title to change it
  jQuery(".navigate-widget-title-customizable").each(function () {
    if (jQuery(this).hasClass('navigate-widget-title-processed')) {
      return;
    }
    jQuery(this).addClass('navigate-widget-title-processed');
    jQuery(this).dblclick(function() {
      var parent = jQuery(this).parent();
      var wid = jQuery(parent).find('.navigate-title-wid').val();
      jQuery(parent).html('<input type="text" class="navigate-title-input" value="' + jQuery(this).text() + '" />');
      jQuery(parent).children().select();
      jQuery(parent).children().bind('keyup', function(e) {
        // If pressing enter
        if (e.which == 13 || e.keyCode == 13) {
          if (jQuery(parent).children().val() == '') {
            alert(Drupal.t('Please enter some value for the title'));
          } else {
            Navigate.variableSet('widget_title', jQuery(parent).children().val(), wid);
            jQuery(parent).html('<div class="navigate-widget-title">' + jQuery(parent).children().val() + '</div>');
            var parent_id = jQuery(parent).attr('id');
            Drupal.attachBehaviors('#' + parent_id);
          }
        }
      });
    });
  });
  
  
  // Bind accordian style
  jQuery('.navigate-accordian-title').each(function () {
    if (jQuery(this).hasClass('navigate-accordian-title-processed')) {
      return;
    }
    jQuery(this).addClass('navigate-accordian-title-processed');
    jQuery(this).click(function() {
      jQuery(this).parents('.navigate-accordian').find('.navigate-accordian-content-outer').slideUp();
      jQuery(this).parent().find('.navigate-accordian-content-outer').slideDown();
    });
  });
  
  
  // Bind actions to 'Load' for single users load / set options
  jQuery('.navigate-default-load-user:not(.navigate-default-load-user-processed)').each(function () {
    jQuery(this).addClass('navigate-default-load-user-processed');
    jQuery(this).click(function () {
      Navigate.queueAdd();
      jQuery.ajax({
        url: Navigate.ajaxUrl(),
        type: 'POST',
        data: 'action=defaults_load&username=' + jQuery('#navigate-set-username').val(),
        error: function() {
        },
        success: function(msg) {
          jQuery('.navigate-defaults').html(msg);
          Navigate.reloadWidgets();
          jQuery('.navigate-defaults').animate({'opacity': 1.0});
          Navigate.queueSubtract();
          Drupal.attachBehaviors('.navigate-defaults');
        }
      });
    });
  });
  
  
  // Bind actions to 'Load' for single users load / set options
  jQuery('.navigate-default-set-user:not(.navigate-default-set-user-processed)').each(function () {
    jQuery(this).addClass('navigate-default-set-user-processed');
    jQuery(this).click(function () {
      if (!confirm('Doing this will erase the current custom set for this user. Are you sure you want to continue?')) {
        return false;
      }
      Navigate.queueAdd();
      jQuery.ajax({
        url: Navigate.ajaxUrl(),
        type: 'POST',
        data: 'action=defaults_set_all&username=' + jQuery('#navigate-set-username').val(),
        error: function() {
        },
        success: function(msg) {
          jQuery('.navigate-defaults').html(msg);
          Navigate.reloadWidgets();
          jQuery('.navigate-defaults').animate({'opacity': 1.0});
          Navigate.queueSubtract();
          Drupal.attachBehaviors('.navigate-defaults');
        }
      });
    });
  });
  
  // Bind keyboard shortcut enable / disable
  jQuery('.navigate-key-button:not(.navigate-key-button-processed)').each(function () {
    jQuery(this).addClass('navigate-key-button-processed');
    
    jQuery(this).click(function () {
      if (jQuery(this).hasClass('key-disabled')) {
        jQuery(document).bind('keydown', {combi:'ctrl+shift+n'});
        jQuery(this).removeClass('key-disabled');
        Navigate.variableSet('key_disabled', '0');
      } else {
        jQuery(document).unbind('keydown', {combi:'ctrl+shift+n'});
        jQuery(this).addClass('key-disabled');
        Navigate.variableSet('key_disabled', '1');
      }
    });
  });
  
  // Bind import / export button
  jQuery('.navigate-import-button:not(.navigate-import-button-procesed)').each(function () {
    jQuery(this).click(function () {
      
    });
  });
  
  }
}


/**
 * Set default item to bind events to
 *
 * If 'item' is not set, this will return .navigate, the class of the div containing the entire navigation
 */
function navigate_item_default(item) {
  item = typeof(item) != 'undefined' ? item : '.navigate ';
  return item + ' ';
}



/**
 * Hide / show navigate bar
 */
Navigate.toggle = function () {
  // Close
  if (navigate_on == 1) {
    Navigate.hide();
  } else {
    // Open
   Navigate.show();
  }
}

/**
 * Hide the navigate bar
 */
Navigate.hide = function () {
  if (navigate_on == 1) {
    navigate_on = 0;
    jQuery(".navigate").stop();
    jQuery("body").animate({ paddingLeft: "0px"}, "fast");
    jQuery(".navigate").animate({ marginLeft: "-550px"}, "fast", function () {Navigate.variableSet('on', '0');});
    jQuery("#navigate-switch").animate({ marginLeft: "-20px", marginTop: "-30px"}, "fast");
    jQuery(".navigate-loading").animate({ marginLeft: "-550px"}, "fast");
    jQuery(".navigate-admin-tools-outer").animate({ marginLeft: "-800px"}, "fast");
  }
}

/**
 * Show the navigate bar
 */
Navigate.show = function () {
  if (navigate_on != 1) {
    var userAgent = self.navigator.userAgent;
    var mleft = "-197px";
    if(userAgent.indexOf('Chrome') > 0) {
      mleft = "13px";
    }
    navigate_on = 1;
    Navigate.variableSet('on', '1');
    jQuery("body").stop();
    jQuery("body").animate({ paddingLeft: "210px"}, "fast");
    jQuery(".navigate").animate({ marginLeft: "-195px"}, "fast");
    jQuery("#navigate-switch").animate({ marginLeft: mleft, marginTop: "0px"}, "fast");
    jQuery(".navigate-loading").animate({ marginLeft: "-30px"}, "fast");
    jQuery(".navigate-admin-tools-outer").animate({ marginLeft: "0px"}, "fast");
  }
}


/**
 * Binds events and various errata after page is loaded
 */
Navigate.bindNavigateOneTime = function () {

  // Make widgets sortable using their title as the handle
  jQuery(".navigate-all-widgets-sortable").sortable({
      cursor: "move",
      distance: 10,
      handle: ".navigate-widget-title",
      revert:true,
      stop:function(e, ui) {
        Navigate.queueAdd();
        jQuery.ajax({
          url: Navigate.ajaxUrl(),
          type: 'POST',
          data: 'action=widget_sort&' + jQuery(".navigate-all-widgets").sortable('serialize'),
          error: function() {
          },
          success: function(msg) {
            Navigate.queueSubtract();
          }
        });
      }
    });
 
   // Bind click event to setting button to show close buttons and new widgets list
  jQuery('.navigate-launch-settings').click(function() {
    jQuery(".navigate-admin-tools-outer").slideToggle('fast', function() { jQuery(this).animate({'opacity': 0.9});});
    jQuery('.navigate-add-widgets').slideToggle('fast', function() {
      jQuery('.navigate-widget-close').toggle();
    });
  });

  // Add select all on text inputs with select all specified
  jQuery(".navigate-select-all").focus(function () {
    jQuery(this).select();
  });
  
  // Set our navigate_on variable
  navigate_on = Drupal.settings.navigateOn;
  
  var userAgent = self.navigator.userAgent;
  var mleft = "-197px";
  if(userAgent.indexOf('Chrome') > 0) {
    mleft = "13px";
  }
    
  // Set initial styling.
  if (navigate_on == 0) {
    jQuery("body").css("padding-left", "0px");
    jQuery("#navigate-switch").css("margin-left", "-20px");
  } else {
    jQuery("body").css("padding-left", "210px");
    jQuery(".navigate").css("margin-left", "-195px");
    jQuery("#navigate-switch").css("margin-left", mleft).css("margin-top", "0px");
    jQuery(".navigate-loading").css("margin-left", "-30px");
  }

  jQuery("#navigate-switch").click(Navigate.toggle);


  // Bind click event to acklowledgement link
  jQuery(".navigate-acknowledgement-switch").click(function () {
    jQuery('.navigate-acknowledgement').slideToggle('fast');
  });

  // Bind doubleclick event to title
  jQuery('.navigate-title').dblclick(function() {
    jQuery('.navigate-shorten').slideToggle('fast');
  });
  
  // Bind help button
  jQuery('.navigate-help-button').click(function () { window.location = jQuery(this).find('.value').val();});
}


/**
 * Bind click actions to on/off buttons.
 */
Navigate.bindToggleButtons = function () {
  jQuery('.navigate-button-outer').each(function() {
    if (!jQuery(this).hasClass('toggle-button-processed')) {
      jQuery(this).addClass('toggle-button-processed');
      var wid = Navigate.getWid(this);
      var widget = this;
      jQuery(this).children().click(function() {
  
        // Grab all variables from hidden inputs
        var on = jQuery(widget).find('.on').val();
        var off = jQuery(widget).find('.off').val();
        var name = jQuery(widget).find('.name').val();
        var required = jQuery(widget).find('.required').val();
        var callback = jQuery(widget).find('.callback').val();
        var val;
  
        // If it's a group
        if (jQuery(widget).find('.group').length > 0) {
          var group = jQuery(widget).find('.group').val();
          if (jQuery(this).hasClass('navigate-button-on')) {
            if (required == 1) {
              return;
            }
            jQuery(this).parents('.navigate-widget').find('.' + group).each(function(child) {
              jQuery(this).removeClass('navigate-button-on');
            });
            jQuery(this).removeClass('navigate-button-on');
            Navigate.variableSet(name, '', wid);
            return;
          }
          jQuery(this).parents('.navigate-widget-outer').find('.' + group).each(function(child) {
            jQuery(this).removeClass('navigate-button-on');
          });
          val = on;
          jQuery(this).addClass('navigate-button-on');
  
        // If not a group
        } else {
          // If it's just a callback
          if (on != '') {
            if (jQuery(this).hasClass('navigate-button-on')) {
              val = off;
              jQuery(this).removeClass('navigate-button-on');
            } else {
              val = on;
              jQuery(this).addClass('navigate-button-on');
            }
          }
        }
        if (callback != '' && callback != undefined) {
          window[callback](wid);
        }
        Navigate.variableSet(name, val, wid);
      });
    }
  });
}


/**
 * Bind callback click actions to buttons.
 */
Navigate.bindCallbackButtons = function () {
  jQuery('.navigate-click-outer').each(function() {
    if (jQuery(this).hasClass('navigate-click-outer-processed')) {
      return;
    }
    jQuery(this).addClass('navigate-click-outer-processed');
    var wid = Navigate.getWid(this);
    var widget = this;
    jQuery(this).children().click(function() {
      var callback = jQuery(widget).children('.callback').val();
      if (callback != '' && callback != undefined) {
        window[callback](wid);
      }
    });
  });
}


/**
 * Bind text input events, including value saving and callback on pressing enter
 */
Navigate.bindTextInputs = function () {
  jQuery('.navigate-text-input-outer').each(function() {
    if (jQuery(this).hasClass('navigate-text-input-processed')) {
      return;
    }
    jQuery(this).addClass('navigate-text-input-processed');
    var wid = Navigate.getWid(this);
    var widget = this;
    var callback = jQuery(widget).children('.callback').val();
    var name = jQuery(widget).children('.name').val();
    var text_input = this;

    jQuery(this).find('input').bind('keyup', function(e) {
      var input_name = jQuery(text_input).children('div').children('input').attr('name');
      var id = jQuery(text_input).children('div').children('input').attr('id');
      var holder_id = name + wid + '_holder';
      // If pressing enter
      if (e.which == 13 || e.keyCode == 13) {
        if (callback != '' && callback != undefined) {
          window[callback](wid);
          if (jQuery(this).hasClass('navigate-clear')) {
            var val = '';
            jQuery(this).val('');
          }
        }
      } 
    });

  });
}


/**
 * Bind textarea save event, including value saving and callback on pressing enter
 */
Navigate.bindTextareas = function () {
  jQuery('.navigate-textarea-outer').each(function() {
    if (jQuery(this).hasClass('navigate-textarea-outer-processed')) {
      return;
    }
    jQuery(this).addClass('navigate-textarea-outer-processed');
    var wid = Navigate.getWid(this);
    var widget = this;
    var callback = jQuery(widget).children('.callback').val();
    var name = jQuery(widget).children('.name').val();
    var text_input = this;
    jQuery(this).find('.navigate-submit').click(function() {
      var val = jQuery(widget).find('.navigate-textarea').val();
      Navigate.variableSet(name, val, wid);
      jQuery(widget).find('.navigate-textarea').html(val);
      if (callback != '' && callback != undefined) {
        window[callback](wid);
      }
    });

  });
}


/**
 * Bind textarea save event, including value saving and callback on pressing enter
 */
Navigate.bindAdminTools = function () {
  if (jQuery('.navigate-admin-tools-defaults').hasClass('navigate-admin-tools-defaults-processed')) {
    return;
  }
  jQuery('.navigate-admin-tools-defaults').addClass('navigate-admin-tools-defaults-processed');
  
  // Set default
  jQuery('.navigate-default-set').click(function () {
    jQuery(this).addClass('navigate-default-set-processed');
    // Save the variable in a database
    Navigate.queueAdd();
    jQuery('.navigate-defaults').animate({'opacity': 0.4});
    jQuery.ajax({
       url: Navigate.ajaxUrl(),
       type: 'POST',
       data: 'action=save_defaults&rid=' + jQuery(this).attr('rel'),
       error: function() {
       },
       success: function(msg) {
        jQuery('.navigate-defaults').html(msg);
        jQuery('.navigate-defaults').animate({'opacity': 1.0});
        Navigate.queueSubtract();
        Drupal.attachBehaviors('.navigate-defaults');
       }
    });
    return false;
  });
  
  // Unset default
  jQuery('.navigate-default-unset').click(function () {
    // Save the variable in a database
    Navigate.queueAdd();
    jQuery('.navigate-defaults').animate({'opacity': 0.4});
    jQuery.ajax({
       url: Navigate.ajaxUrl(),
       type: 'POST',
       data: 'action=defaults_unset&rid=' + jQuery(this).attr('rel'),
       error: function() {
       },
       success: function(msg) {
        jQuery('.navigate-defaults').html(msg);
        jQuery('.navigate-defaults').animate({'opacity': 1.0});
        Navigate.queueSubtract();
        Drupal.attachBehaviors('.navigate-defaults');
       }
    });
    return false;
  });
  
  // Load default
  jQuery('.navigate-default-load').click(function () {
    // Save the variable in a database
    Navigate.queueAdd();
    jQuery('.navigate-defaults').animate({'opacity': 0.4});
    jQuery.ajax({
       url: Navigate.ajaxUrl(),
       type: 'POST',
       data: 'action=defaults_load&rid=' + jQuery(this).attr('rel'),
       error: function() {
       },
       success: function(msg) {
        jQuery('.navigate-defaults').html(msg);
        Navigate.reloadWidgets();
        jQuery('.navigate-defaults').animate({'opacity': 1.0});
        Navigate.queueSubtract();
        Drupal.attachBehaviors('.navigate-defaults');
       }
    });
    return false;
  });
  
  // Switch back to saved set
  jQuery('.navigate-default-switch-back').click(function () {
    // Save the variable in a database
    Navigate.queueAdd();
    jQuery('.navigate-defaults').animate({'opacity': 0.4});
    jQuery.ajax({
       url: Navigate.ajaxUrl(),
       type: 'POST',
       data: 'action=defaults_switch_back',
       error: function() {
       },
       success: function(msg) {
        jQuery('.navigate-defaults').html(msg);
        Navigate.reloadWidgets();
        jQuery('.navigate-defaults').animate({'opacity': 1.0});
        Navigate.queueSubtract();
        Drupal.attachBehaviors('.navigate-defaults');
       }
    });
    return false;
  });
  
  // Switch back to saved set
  jQuery('.navigate-default-set-all').click(function () {
    if (!confirm('Doing this will erase the current custom set for all users in this role, and will set this set as the default for this role. Are you sure you want to continue?')) {
      return false;
    }
    // Save the variable in a database
    Navigate.queueAdd();
    jQuery('.navigate-defaults').animate({'opacity': 0.4});
    jQuery.ajax({
       url: Navigate.ajaxUrl(),
       type: 'POST',
       data: 'action=defaults_set_all&rid=' + jQuery(this).attr('rel'),
       error: function() {
       },
       success: function(msg) {
        jQuery('.navigate-defaults').html(msg);
        Navigate.reloadWidgets();
        jQuery('.navigate-defaults').animate({'opacity': 1.0});
        Navigate.queueSubtract();
        Drupal.attachBehaviors('.navigate-defaults');
       }
    });
    return false;
  });
  
  // Bind user search check
  jQuery('#navigate-set-username').bind('keyup',function () {
    if (jQuery(this).val() != '') {
      jQuery('.navigate-default-set-user-load').html('<a class="navigate-default-load-user" href="javascript:;">' + Drupal.t('Load') + '</a>');
      jQuery('.navigate-default-set-user-set').html('<a class="navigate-default-set-user" href="javascript:;">' + Drupal.t('Set user') + '</a>');
    } else {
      jQuery('.navigate-default-set-user-load').html('<span class="default-disabled">' + Drupal.t('Load') + '</span>');
      jQuery('.navigate-default-set-user-set').html('<span class="default-disabled">' + Drupal.t('Set user') + '</span>');
    }
    Drupal.attachBehaviors('.navigate-defaults-table');
  });
  
  // Add instructions to empty username search
  jQuery('#navigate-set-username').focus(function () {
    jQuery(this).removeClass('navigate-username-empty');
    if (jQuery(this).val() == Drupal.t('Search name / UID')) {
      jQuery(this).val('');
    }
  });
  
  // Add instructions to empty username search
  jQuery('#navigate-set-username').blur(function () {
    if (jQuery(this).val() == '') {
      jQuery(this).addClass('navigate-username-empty');
      jQuery(this).val(Drupal.t('Search name / UID'));
    }
  });
}


/**
 * Reload navigate widget set
 */
Navigate.reloadWidgets = function () {
  jQuery('.navigate-all-widgets').animate({'opacity': 0.4});
  jQuery.ajax({
      url: Navigate.ajaxUrl(),
      type: 'POST',
      data: 'action=widgets_reload',
      error: function() {
      },
      success: function(msg) {
       jQuery('.navigate-all-widgets').html(msg);
       jQuery('.navigate-all-widgets').animate({'opacity': 1.0});
       Navigate.queueSubtract();
       jQuery('.navigate-widget-close').show();
       Drupal.attachBehaviors('.navigate-all-widgets');
      }
   });
}

/**
 * Set a variable
 */
Navigate.variableSet = function (name, val, wid) {
  // Save the variable in a database
  Navigate.queueAdd();
   jQuery.ajax({
      url: Navigate.ajaxUrl(),
      type: 'POST',
      data: 'action=variable_save&name=' + name + '&wid=' + wid + '&value=' + encodeURIComponent(val),
      error: function() {
      },
      success: function(msg) {
        Navigate.queueSubtract();
      }
  });
}

/**
 * Add count to loader queue
 */
Navigate.queueAdd = function () {
  if (Navigate.loaderCount < 1) {
    jQuery(".navigate-loading").show();
  }
  Navigate.loaderCount++;
}

/**
 * Remove count from loader
 */
Navigate.queueSubtract = function () {
  Navigate.loaderCount--;
  if (Navigate.loaderCount < 1) {
    jQuery(".navigate-loading").slideUp();
  }
}


/**
 * Grab the wid of a widget from inside the widget
 */
Navigate.getWid = function (item) {
  return jQuery(item).parents('.navigate-widget-outer').children('.wid').val();
}


/**
 * Get the location of the current URL
 */
Navigate.ajaxUrl = function () {
  return Drupal.settings.basePath + 'navigate/process'
}


/**
 * Send request to navigate processing function
 *
 * This is used to run widget callbacks through, so we can sanitize query string data.
 */
Navigate.processAjax = function (wid, query_array, callback) {
  var query_string = '';
  for(var i in query_array) {
    if (i != '') {
      query_string += i + '=';
    }
    query_string += query_array[i] + '&';
  }
  query_string += '&wid=' + wid + '&return=' + jQuery('#navigate_q').val();
  Navigate.queueAdd();
  jQuery.ajax({
      url: Navigate.ajaxUrl(),
      type: 'POST',
      data: query_string,
      error: function() {
      },
      success: function(msg) {
        Navigate.queueSubtract();
        if (callback != undefined) {
          window[callback](msg, wid);
        }
      }
  });
}


/**
 * Binds click event to list of widgets
 */
Navigate.bindAddWidgetList = function () {
  if (jQuery('.navigate-widget-list').hasClass('navigate-widget-list-processed')) {
    return;
  }
  jQuery('.navigate-widget-list').addClass('navigate-widget-list-processed');
  jQuery('.navigate-widget-list-item').click(function() {
    $list_item = jQuery(this);
    
    jQuery('.navigate-widget-list-item').addClass('navigate-widget-list-processed');
    
    // If it's a one-at-a-time widget, return false
    if (jQuery(this).find('.single').val() == 1) {
      if (jQuery('.navigate-widget-outer > .type[value="' + jQuery(this).attr('id') + '"]').length > 0) {
        alert(t('Sorry, only one instance of this widget is allowed at one time.'));
        return false;
      }
    }
    Navigate.queueAdd();
    jQuery.ajax({
      url: Navigate.ajaxUrl(),
      type: 'POST',
      dataType: 'html',
      data: 'action=add_widget&module=' + jQuery(this).attr('title') + '&type=' + jQuery(this).attr('id'),
      error: function() {
      },
      success: function(msg) {
        Navigate.queueSubtract();
        jQuery('.navigate-all-widgets').append(msg);
        var widget = '#' + jQuery(msg).parent().children('.navigate-widget-outer').attr('id');
        funct = $list_item.find('.callback').val();
        if (funct != '' && funct != undefined) {
          window[funct](widget);
        }
        Drupal.attachBehaviors();
      }
    });
    return true;
  });
}

/**
 * Create export content and populate textarea
 */
function navigate_set_export() {
  jQuery('.navigate-import-export').animate({'opacity': 0.4});
  jQuery.ajax({
    url: Navigate.ajaxUrl(),
    type: 'POST',
    dataType: 'html',
    data: 'action=export_widget_set',
    error: function() {
    },
    success: function(msg) {
      Navigate.queueSubtract();
      jQuery('#navigate-favorites-export_').val(msg);
      jQuery('.navigate-import-export').animate({'opacity': 1.0});
      Drupal.attachBehaviors();
    }
  });
}

/**
 * Create export content and populate textarea
 */
function navigate_set_import() {
  jQuery('.navigate-import-export').animate({'opacity': 0.4});
  jQuery.ajax({
    url: Navigate.ajaxUrl(),
    type: 'POST',
    dataType: 'html',
    data: 'action=import_widget_set&import=' + jQuery('#navigate-favorites-import_').val(),
    error: function() {
    },
    success: function(msg) {
      Navigate.queueSubtract();
      jQuery('#navigate-favorites-import_').val('');
      jQuery('.navigate-import-export').animate({'opacity': 1.0});
      Navigate.reloadWidgets();
      Drupal.attachBehaviors();
    }
  });
}
