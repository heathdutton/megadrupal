(function ($) {

/**
 * curlypage edit form fields js
 */
Drupal.behaviors.curlypage = {
  attach: function(context, settings) {

    // Disable absolute position model if curlypage is set on bottom corner
    $('#edit-peel-position', context).click(function () {
      if ($('#edit-peel-position-bottomleft').attr('checked') || $('#edit-peel-position-bottomright').attr('checked')) {
        $('#edit-peel-position-model-absolute').attr('disabled', true);
        $('#edit-peel-position-model-fixed').attr('checked', true);
      } else {
        $('#edit-peel-position-model-absolute').removeAttr('disabled');
      }
    });

    if ($('#edit-peel-position-bottomleft').attr('checked') || $('#edit-peel-position-bottomright').attr('checked')) {
      $('#edit-peel-position-model-absolute').attr('disabled', true);
      $('#edit-peel-position-model-fixed').attr('checked', true);
    }

    // transition duration slider
    $("#edit-transition-duration").keyup(function(event) {
	  if (!isNaN(parseInt($("#edit-transition-duration").val())) && parseInt($("#edit-transition-duration").val()) > 0) {
        $("#transition-duration-slider").slider("value", $("#edit-transition-duration").val());
      } else {
        if ($("#edit-transition-duration").val() != "") {
          $("#edit-transition-duration").val($("#transition-duration-slider").slider("value"));
        }
      }
    });

    $("#edit-transition-duration").change(function(event) {
      if ($("#edit-transition-duration").val() == "") {
        $("#edit-transition-duration").val(1);
        $("#transition-duration-slider").slider("value", 1);
      }
    });

    $("#transition-duration-callout").hide();

    $("#transition-duration-slider").slider({
      animate: true,
      min: 1,
      max: 9,
      value: $("#edit-transition-duration").attr("value"),
      start: function(event, ui) {
        $("#transition-duration-callout").fadeIn("fast");
      },
      stop: function(event, ui) {
        $("#transition-duration-callout").fadeOut("fast");
      },
      slide: function(event, ui) {
        $("#edit-transition-duration").attr("value", ui.value);
        $("#transition-duration-callout").css("left", (ui.value - 1)*160/8).text(ui.value);
      }
    });

    // custom color picker
    $.farbtastic("#curlypage-customcolor-picker", "#edit-custom-color");

    // flag speed slider
    $("#edit-flag-speed").keyup(function(event) {
      if (!isNaN(parseInt($("#edit-flag-speed").val())) && parseInt($("#edit-flag-speed").val()) > 0) {
        $("#flag-speed-slider").slider("value", $("#edit-flag-speed").val());
      } else {
        if ($("#edit-flag-speed").val() != "") {
          $("#edit-flag-speed").val($("#flag-speed-slider").slider("value"));
        }
      }
    });

    $("#edit-flag-speed").change(function(event) {
      if ($("#edit-flag-speed").val() == "") {
        $("#edit-flag-speed").val(1); $("#flag-speed-slider").slider("value", 1);
      }
    });

    $("#flag-speed-callout").hide();

    $("#flag-speed-slider").slider({
      animate: true,
      min: 1,
      max: 9,
      value: $("#edit-flag-speed").attr("value"),
      start: function(event, ui) {
        $("#flag-speed-callout").fadeIn("fast");
      },
      stop: function(event, ui) {
        $("#flag-speed-callout").fadeOut("fast");
      },
      slide: function(event, ui) {
        $("#edit-flag-speed").attr("value", ui.value);
        $("#flag-speed-callout").css("left", (ui.value - 1)*160/8).text(ui.value);
      }
    });

    // peel speed slider
    $("#edit-peel-speed").keyup(function(event) {
      if (!isNaN(parseInt($("#edit-peel-speed").val())) && parseInt($("#edit-peel-speed").val()) > 0) {
        $("#peel-speed-slider").slider("value", $("#edit-peel-speed").val());
      } else {
        if ($("#edit-peel-speed").val() != "") {
          $("#edit-peel-speed").val($("#peel-speed-slider").slider("value"));
        }
      }
    });

    $("#edit-peel-speed").change(function(event) {
      if ($("#edit-peel-speed").val() == "") {
        $("#edit-peel-speed").val(1); $("#peel-speed-slider").slider("value", 1);
      }
    });

    $("#peel-speed-callout").hide();

    $("#peel-speed-slider").slider({
      animate: true,
      min: 1,
      max: 9,
      value: $("#edit-peel-speed").attr("value"),
      start: function(event, ui) {
        $("#peel-speed-callout").fadeIn("fast");
      },
      stop: function(event, ui) {
        $("#peel-speed-callout").fadeOut("fast");
      },
      slide: function(event, ui) {
        $("#edit-peel-speed").attr("value", ui.value);
        $("#peel-speed-callout").css("left", (ui.value - 1)*160/8).text(ui.value);
      }
    });

    // automatic open
    $("#edit-automatic-open").keyup(function(event) {
      if (!isNaN(parseInt($("#edit-automatic-open").val()))) {
        $("#automatic-open-slider").slider("value", $("#edit-automatic-open").val());
      } else {
        if ($("#edit-automatic-open").val() != "") {
          $("#edit-automatic-open").val($("#automatic-open-slider").slider("value"));
        }
      }
    });

    $("#edit-automatic-open").change(function(event) {
      if ($("#edit-automatic-open").val() == "") {
        $("#edit-automatic-open").val(0); $("#automatic-open-slider").slider("value", 0);
      }
    });

    $("#automatic-open-callout").hide();

    $("#automatic-open-slider").slider({
      animate: true,
      min: 0,
      max: 999,
      value: $("#edit-automatic-open").attr("value"),
      start: function(event, ui) {
        $("#automatic-open-callout").fadeIn("fast");
      },
      stop: function(event, ui) {
        $("#automatic-open-callout").fadeOut("fast");
      },
      slide: function(event, ui) {
        $("#edit-automatic-open").attr("value", ui.value);
        $("#automatic-open-callout").css("left", ui.value*500/1000).text(ui.value);
      }
    });

    //automatic close
    $("#edit-automatic-close").keyup(function(event) {
      if (!isNaN(parseInt($("#edit-automatic-close").val()))) {
        $("#automatic-close-slider").slider("value", $("#edit-automatic-close").val());
      } else {
        if ($("#edit-automatic-close").val() != "") {
          $("#edit-automatic-close").val($("#automatic-close-slider").slider("value"));
        }
      }
    });

    $("#edit-automatic-close").change(function(event) {
      if ($("#edit-automatic-close").val() == "") {
        $("#edit-automatic-close").val(0);
        $("#automatic-close-slider").slider("value", 0);
      }
    });

    $("#automatic-close-callout").hide();

    $("#automatic-close-slider").slider({
      animate: true,
      min: 0,
      max: 999,
      value: $("#edit-automatic-close").attr("value"),
      start: function(event, ui) {
        $("#automatic-close-callout").fadeIn("fast");
      },
      stop: function(event, ui) {
        $("#automatic-close-callout").fadeOut("fast");
      },
      slide: function(event, ui) {
        $("#edit-automatic-close").attr("value", ui.value);
        $("#automatic-close-callout").css("left", ui.value*500/1000).text(ui.value);
      }
    });

    // close button color
    $.farbtastic("#curlypage-closebuttoncolor-picker", "#edit-close-button-color");

    // delay
    $("#edit-delay").keyup(function(event) {
      if (!isNaN(parseInt($("#edit-delay").val()))) {
        $("#delay-slider").slider("value", $("#edit-delay").val());
      } else {
        if ($("#edit-delay").val() != "") {
          $("#edit-delay").val($("#delay-slider").slider("value"));
        }
      }
    });

    $("#edit-delay").change(function(event) {
      if ($("#edit-delay").val() == "") {
        $("#edit-delay").val(0);
        $("#delay-slider").slider("value", 0);
      }
    });

    $("#delay-callout").hide();

    $("#delay-slider").slider({ animate: true,
      min: 0,
      max: 999,
      value: $("#edit-delay").attr("value"),
      start: function(event, ui) {
        $("#delay-callout").fadeIn("fast");
      },
      stop: function(event, ui) {
        $("#delay-callout").fadeOut("fast");
      },
      slide: function(event, ui) {
        $("#edit-delay").attr("value", ui.value);
        $("#delay-callout").css("left", ui.value*500/1000).text(ui.value);
      }
    });

    // time slot
    $("#edit-time-slot").keyup(function(event) {
      if (!isNaN(parseInt($("#edit-time-slot").val()))) {
        $("#time-slot-slider").slider("value", $("#edit-time-slot").val());
      } else {
        if ($("#edit-time-slot").val() != "") {
          $("#edit-time-slot").val($("#time-slot-slider").slider("value"));
        }
      }
    });

    $("#edit-time-slot").change(function(event) {
      if ($("#edit-time-slot").val() == "") {
        $("#edit-time-slot").val(0);
        $("#time-slot-slider").slider("value", 0);
      }
    });

    $("#time-slot-callout").hide();

    $("#time-slot-slider").slider({
      animate: true,
      min: 0,
      max: 999,
      value: $("#edit-time-slot").attr("value"),
      start: function(event, ui) {
        $("#time-slot-callout").fadeIn("fast");
      },
      stop: function(event, ui) {
        $("#time-slot-callout").fadeOut("fast");
      },
      slide: function(event, ui) {
        $("#edit-time-slot").attr("value", ui.value);
        $("#time-slot-callout").css("left", ui.value*500/1000).text(ui.value);
      }
    });

    // repeat
    $("#edit-repeat-times").keyup(function(event) {
      if (!isNaN(parseInt($("#edit-repeat-times").val()))) {
        $("#repeat-times-slider").slider("value", $("#edit-repeat-times").val());
      } else {
        if ($("#edit-repeat-times").val() != "") {
          $("#edit-repeat-times").val($("#repeat-times-slider").slider("value"));
        }
      }
    });

    $("#edit-repeat-times").change(function(event) {
      if ($("#edit-repeat-times").val() == "") {
        $("#edit-repeat-times").val(0);
        $("#repeat-times-slider").slider("value", 0);
      }
    });

    $("#repeat-times-callout").hide();

    $("#repeat-times-slider").slider({
      animate: true,
      min: 0,
      max: 9,
      value: $("#edit-repeat-times").attr("value"),
      start: function(event, ui) {
        $("#repeat-times-callout").fadeIn("fast");
      },
      stop: function(event, ui) {
        $("#repeat-times-callout").fadeOut("fast");
      },
      slide: function(event, ui) {
        $("#edit-repeat-times").attr("value", ui.value);
        $("#repeat-times-callout").css("left", ui.value*180/9).text(ui.value);
      }
    });

  }
}

/**
 * Provide the summary information for the curlypage settings vertical tabs.
 */
Drupal.behaviors.curlypageSettingsSummary = {
  attach: function (context) {
    // The drupalSetSummary method required for this behavior is not available
    // on the Curlypages administration page, so we need to make sure this
    // behavior is processed only if drupalSetSummary is defined.
    if (typeof jQuery.fn.drupalSetSummary == 'undefined') {
      return;
    }

    $('fieldset#edit-position-settings', context).drupalSetSummary(function (context) {
      var vals = [];
      var corner = $('input[name="peel_position"]:checked').next('label').text();
      vals.push(corner);
      var model = $('input[name="peel_position_model"]:checked').next('label').text();
      vals.push(model);
      return vals.join(', ');
    });

    $('fieldset#edit-wait-settings', context).drupalSetSummary(function (context) {
      var vals = [];
      if (!$('input[name="wait_enable"]:checked', context).val()) {
        return Drupal.t('Disabled');
      }
      return Drupal.t('Enabled');
    });

    $('fieldset#edit-style-settings', context).drupalSetSummary(function (context) {
      var vals = [];
      var flagStyle = $('select[name="flag_style"] option:selected').text();
      vals.push(Drupal.t('Flag: @value', { '@value': flagStyle }));
      var peelStyle = $('select[name="peel_style"] option:selected').text();
      vals.push(Drupal.t('Peel: @value', { '@value': peelStyle }));
      return vals.join('<br />');
    });

    $('fieldset#edit-size-settings', context).drupalSetSummary(function (context) {
      var vals = [];
      var flagDimension = $('input[name="flag_width"]').val() + 'x' + $('input[name="flag_height"]').val();
      vals.push(Drupal.t('Flag: @value', { '@value' : flagDimension }));
      var peelDimension = $('input[name="peel_width"]').val() + 'x' + $('input[name="peel_height"]').val();
      vals.push(Drupal.t('Peel: @value', { '@value' : peelDimension }));
      return vals.join('<br />');
    });

    $('fieldset#edit-image-settings', context).drupalSetSummary(function (context) {
      var vals = [];

      if ($('input[name="mirror"]:checked').val()) {
        vals.push(Drupal.t('Back mirror enabled'));
      }

      var transition = $('select[name="in_transition"] option:selected').text();
      var duration = $('input[name="transition_duration"]').val();
      vals.push(Drupal.t('Transition: @value1 / @value2', { '@value1' : transition , '@value2' : duration }));

      var style = $('input[name="peel_color_style"]:checked').next('label').text();
      var color = $('input[name="peel_color"]:checked').next('label').text();
      var isCustom = $('input[name="peel_color"]:checked').val() == 'custom';
      var custom = $('input[name="custom_color"]').val();

      if (isCustom) {
        vals.push(Drupal.t('Color: @value1 @value2 @value3', { '@value1' : style , '@value2' : color , '@value3' : custom }));
      } else {
        vals.push(Drupal.t('Color: @value1 @value2', { '@value1' : style , '@value2' : color }));
      }

      return vals.join("<br />");
    });

    $('fieldset#edit-sound-settings', context).drupalSetSummary(function (context) {
      var vals = [];

      if ($('input[name="load_sound_remove"]').length) {
        vals.push(Drupal.t('Load sound enabled'));
      }
      if ($('input[name="open_sound_remove"]').length) {
        vals.push(Drupal.t('Open sound enabled'));
      }
      if ($('input[name="close_sound_remove"]').length) {
        vals.push(Drupal.t('Close sound enabled'));
      }
      if (!vals.length) {
        return Drupal.t('No sounds');
      }
      return vals.join('<br />');
    });

    $('fieldset#edit-linking-settings', context).drupalSetSummary(function (context) {
      var vals = [];
      if (!$('input[name="link_enabled"]:checked', context).val()) {
        return Drupal.t('Disabled');
      }
      var link = $('input[name="link"]').val();
      vals.push(Drupal.t('URL: @value', { '@value': link }));
      var target = $('input[name="link_target"]:checked').next('label').text();
      vals.push(Drupal.t('Target: @value', { '@value': target }));
      return vals.join('<br />');
    });

    $('fieldset#edit-motion-settings', context).drupalSetSummary(function (context) {
      var vals = [];
      var flagSpeed = $('input[name="flag_speed"]').val();
      vals.push(Drupal.t('Flag Speed: @value', { '@value': flagSpeed }));
      var peelSpeed = $('input[name="peel_speed"]').val();
      vals.push(Drupal.t('Peel Speed: @value', { '@value': peelSpeed }));
      var automaticOpen = $('input[name="automatic_open"]').val();
      vals.push(Drupal.t('Automatic Open: @value sec.', { '@value': automaticOpen }));
      var automaticClose = $('input[name="automatic_close"]').val();
      vals.push(Drupal.t('Automatic Close: @value sec.', { '@value': automaticClose }));
      return vals.join('<br />');
    });

    $('fieldset#edit-closebutton-settings', context).drupalSetSummary(function (context) {
      var vals = [];
      if (!$('input[type="checkbox"]:checked', context).val()) {
        return Drupal.t('Disabled');
      }
      var text = $('input[name="text_on_close_button"]').val();
      vals.push(Drupal.t('Text: @value', { '@value': text }));
      var color = $('input[name="close_button_color"]').val();
      vals.push(Drupal.t('Color: @value', { '@value': color }));
      return vals.join('<br />');
    });

    $('fieldset#edit-time-settings', context).drupalSetSummary(function (context) {
      var vals = [];
      var delay = $('input[name="delay"]').val();
      vals.push(Drupal.t('Delay: @value sec.', { '@value': delay }));
      var timeSlot = $('input[name="time_slot"]').val();
      vals.push(Drupal.t('Time Slot: @value sec.', { '@value': timeSlot }));
      var repeatTimes = $('input[name="repeat_times"]').val();
      vals.push(Drupal.t('Repeat: @value times', { '@value': repeatTimes }));
      return vals.join('<br />');
    });

    $('fieldset#edit-page-vis-settings', context).drupalSetSummary(function (context) {
      if (!$('textarea[name="pages"]', context).val()) {
        return Drupal.t('Not restricted');
      }
      else {
        return Drupal.t('Restricted to certain pages');
      }
    });

    $('fieldset#edit-node-types-vis-settings', context).drupalSetSummary(function (context) {
      var vals = [];
      $('input[type="checkbox"]:checked', context).each(function () {
        vals.push($.trim($(this).next('label').text()));
      });
      if (!vals.length) {
        return Drupal.t('Not restricted');
      }
      return Drupal.t('Show on:') + " " + vals.join(', ');
    });

    $('fieldset#edit-languages-vis-settings', context).drupalSetSummary(function (context) {
      var vals = [];
      $('input[type="checkbox"]:checked', context).each(function () {
        vals.push($.trim($(this).next('label').text()));
      });
      if (!vals.length) {
        return Drupal.t('Not restricted');
      }
      return vals.join(', ');
    });

    $('fieldset#edit-roles-vis-settings', context).drupalSetSummary(function (context) {
      var vals = [];
      $('input[type="checkbox"]:checked', context).each(function () {
        vals.push($.trim($(this).next('label').text()));
      });
      if (!vals.length) {
        return Drupal.t('Not restricted');
      }
      var $radio = $('input[name="roles_visibility"]:checked', context);
      if ($radio.val() == 0) {
        return Drupal.t('Show to:') + " " + vals.join(', ');
      }
      else {
        return Drupal.t('Show except to:') + " " + vals.join(', ');
      }
    });

  }
};

})(jQuery);
