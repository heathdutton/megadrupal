/**
 * @file
 * Managing asynchronous calls to server.
 */

/* exported validate_data, validate_individual, bulk_author_update */
/* exported bulk_author_update_suggest_user */
function validate_data(whichthis) {
  "use strict";
  if (whichthis.id === "parent_check") {
    if (whichthis.checked) {
      jQuery(".child_check").each(function () {
        jQuery(".child_check").attr('checked', true);
      });
    }
    else {
      jQuery(".child_check").each(function () {
        jQuery(".child_check").attr('checked', false);
      });
    }
  }
}

function validate_individual(whichthis) {
  "use strict";
  jQuery(".parent_check").attr('checked', false);
  var counter = 0;
  var count = 0;
  jQuery(".child_check").each(function () {
    count++;
    if (this.checked) {
      counter++;
    }
  });
  if (counter === 0) {
    jQuery(".parent_check").attr('checked', false);
  }
  if (counter === count) {
    jQuery(".parent_check").attr('checked', true);
  }
}

function bulk_author_update() {
  "use strict";
  var collect = [];
  jQuery(".child_check").each(function () {
    if (this.checked) {
      collect.push(this.value);
    }
  });
  if (collect.length === 0) {
    alert(Drupal.settings.vars.validation_message);
    return false;
  }
  var user = document.getElementsByName("suggest_user");
  for (var i = 0; i < user.length; i++) {
    if (user[i].value === "") {
      alert(Drupal.settings.vars.user_message);
      return false;
    }
  }
  var selected_user = jQuery("#selected_user").val();
  if (selected_user === "" || selected_user === null) {
    alert(Drupal.settings.vars.user_not_found);
    return false;
  }
  var selected_data = collect.join(",");
  var input_obj = {};
  input_obj.selected_data = selected_data;
  input_obj.selected_user = selected_user;
  show_ajax_loader();
  jQuery.ajax({
    url: Drupal.settings.vars.request_url,
    type: 'POST',
    data: input_obj,
    dataType: 'json',
    async: true,
    success: function (result) {
      hide_ajax_loader();
      window.location = Drupal.settings.vars.redirect_url;
    },
    error: function (xhr, ajaxOptions, thrownError) {
      setTimeout(function () {
        hide_ajax_loader();
        alert(Drupal.settings.vars.error_message);
        return false;
      }, 200);
      return false;
    }
  });

}

jQuery('document').ready(function () {
  "use strict";
  jQuery('#auto_list_container ul li').live('click', function () {
    var input_obj = {};
    input_obj.user_id = this.id;
    show_ajax_loader();
    jQuery.ajax({
      url: Drupal.settings.vars.request_url,
      type: 'POST',
      data: input_obj,
      dataType: 'json',
      async: true,
      success: function (result) {
        hide_ajax_loader();
        jQuery("#auto_list").css('border', 'none');
        jQuery(".description").hide();
        jQuery("#auto_list").html('');
        jQuery("#details_container").html(result);
        var selected_user_name = jQuery("#selected_user_name").val();
        var modified_url = jQuery("#link_update").val();
        jQuery("#edit-suggest-user").val(selected_user_name);
        jQuery("#update_all a").attr('href', modified_url);
        jQuery("#update_all").css('background', '#808080');
        jQuery("#update_all a").css('color', '#FFFFFF');
      },
      error: function (xhr, ajaxOptions, thrownError) {
        setTimeout(function () {
          hide_ajax_loader();
          alert(Drupal.settings.vars.error_message);

        }, 200);
        return false;
      }
    });
  });
});

function bulk_author_update_suggest_user(whichthis) {
  "use strict";
  var user_name = whichthis.value;
  if (user_name !== null || user_name !== '') {
    var input_obj = {};
    input_obj.user_name = user_name;
    jQuery.ajax({
      url: Drupal.settings.vars.request_url,
      type: 'POST',
      data: input_obj,
      dataType: 'json',
      async: true,
      success: function (result) {
        jQuery(".description").hide();
        jQuery("#auto_list").css({
          'border-left': '1px solid silver',
          'border-right': '1px solid silver',
          'border-bottom': '1px solid silver',
          'position': 'absolute',
          'background': '#FFF',
          'top': '-5px'
        });
        jQuery("#auto_list").html(result);
      },
      error: function (xhr, ajaxOptions, thrownError) {
        setTimeout(function () {
          alert(Drupal.settings.vars.error_message);
        }, 200);
        return false;
      }
    });
  }

}

function show_ajax_loader() {
  "use strict";
  // Added to show overlay div after form submit.
  var windowTotalHeight = jQuery(window).height() + jQuery(window).scrollTop();
  var windowActualHeight = jQuery(window).scrollTop();
  var overlay_black = 'position:absolute;top:0%;left:0%;width:100%;height:' + windowTotalHeight + 'px;background:white;z-index:99999;opacity:0.1;filter: alpha(opacity=10);';
  jQuery('body').append('<div id="overlayBlack" style="' + overlay_black + '"></div>');
  var overlay_white = 'position:absolute; top:' + windowActualHeight + 'px; left:60%;  z-index:1002; overflow:auto;  margin-left:0px; margin-top:200px;';
  jQuery('body').append('<div id="ajaxLoder" style="' + overlay_white + '" ><img src="' + Drupal.settings.vars.ajax_image + '" alt="loading..." /></div>');
}

function hide_ajax_loader() {
  "use strict";
  jQuery('#overlayBlack').remove();
  jQuery('#ajaxLoder').remove();
}
