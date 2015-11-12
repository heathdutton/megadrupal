jQuery(document).ready(function ($) {
  var change_flag = false;
  //ugly fix for AHAH (add more button can't have same name) @ http://drupal.org/node/1342066
  $("#edit-anonymous-blazemeter-ahah-anon-page").val("Add Page");
  $("#edit-authenticated-blazemeter-ahah-auth-page").val("Add Page");

  var max_users = parseInt($("#edit-max-users").val());
  var median_users = max_users / 2;
  //anon slider
  $("#anon-slider").slider({
    min:0,
    max:max_users,
    range:"min",
    value:$("#edit-anonymous-anon").val(),
    slide:function (event, ui) {
      $("#edit-anonymous-anon").val(ui.value);
      change_flag = true;
    }
  });
  $("#edit-anonymous-anon").change(function () {
    if (isNaN($("#edit-anonymous-anon").val())) {
      //User entered a string
      $("#edit-anonymous-anon").val(median_users);
    }
    if ($("#edit-anonymous-anon").val() > max_users) {
      $("#edit-anonymous-anon").val(max_users);
    }
    if ($("#edit-anonymous-anon").val() < 0) {
      $("#edit-anonymous-anon").val(0);
    }
    $("#anon-slider").slider("value", $("#edit-anonymous-anon").val());
  });

  //auth slider
  $("#auth-slider").slider({
    min:0,
    max:max_users,
    range:"min",
    value:$("#edit-authenticated-auth").val(),
    slide:function (event, ui) {
      $("#edit-authenticated-auth").val(ui.value);
      change_flag = true;
    }
  });
  $("#edit-authenticated-auth").change(function () {
    if (isNaN($("#edit-authenticated-auth").val())) {
      //User entered a string
      $("#edit-authenticated-auth").val(0);
    }
    if ($("#edit-authenticated-auth").val() > max_users) {
      $("#edit-authenticated-auth").val(max_users);
    }
    if ($("#edit-authenticated-auth").val() < 0) {
      $("#edit-authenticated-auth").val(0);
    }
    $("#auth-slider").slider("value", $("#edit-authenticated-auth").val());
  });

  $("#blazemeter-signup").click(function () {
    $('#blazemeter-signup-modal').modal({
      closeHTML:"<a href='#' title='Close' class='modal-close'>x</a>",
      height:605,
      minHeight:605,
      containerId:'simplemodal-register-container'
    });
    //Check if the user key is set.
    window.setTimeout(function () {
      blazemeter_userkey_check();
    }, 5000);
  });

  $("#blazemeter-login").click(function () {
    $('#blazemeter-login-modal').modal({
      closeHTML:"<a href='#' title='Close' class='modal-close'>x</a>",
      height:503,
      minHeight:503,
      containerId:'simplemodal-login-container'
    });

    //Check if the user key is set.
    window.setTimeout(function () {
      blazemeter_userkey_check();
    }, 5000);
  });

  //Scenario
  $("#blazemeter-scenario .blazemeter-button").click(function () {
    var id = $(this).attr("id");
    if (id != $("#edit-meta-scenario").val()) {
      change_flag = true;
    }
    switch (id) {
      case "blazemeter-scenario-load":
        $("#edit-meta-scenario").val("load");
        break;
      case "blazemeter-scenario-stress":
        $("#edit-meta-scenario").val("stress");
        break;
      case "blazemeter-scenario-extreme":
        $("#edit-meta-scenario").val("extreme stress");
        break;
    }

    $("#blazemeter-scenario .blazemeter-button").removeClass("button-selected");
    $(this).addClass("button-selected");
  });

  if ($("#edit-meta-hasuserkey").val()) {
    $("#edit-meta-userkey-holder").val("user key is stored");
  }

  //Tooltips for scenario description
  $("#blazemeter-scenario-load").tooltip({position:"top right", relative:true, offset:[150, 250]});
  $("#blazemeter-scenario-stress").tooltip({position:"top right", relative:true, offset:[150, 192]});
  $("#blazemeter-scenario-extreme").tooltip({position:"top right", relative:true, offset:[150, 75]});

  $('#edit-meta-userkey-holder').keyup(function () {
    if ($('#password-password').val() != '') {
      $('#edit-meta-userkey').val($('#edit-meta-userkey-holder').val());
    }
  });

  $("#blazemeter-admin-settings-form #edit-buttons-goto").click(function () {
    $("#blazemeter-admin-settings-form .warning").remove();
    if (change_flag) {
      $("#blazemeter-admin-settings-form .submit-buttons").before("<div class='messages warning'>* The changes will not be saved until the Save button is clicked.</div>");
    }
  });

  $('#blazemeter-admin-settings-form').change(function () {
    change_flag = true;
  });

  function blazemeter_userkey_check() {
    $.ajax({
      type:"GET",
      url:Drupal.settings.basePath + "?q=blazemeter_ajax/userkey",
      dataType:'json',
      success:function (data) {
        if (data.status) {
          //Userkey is sucessfully stored
          $(".modal-close").click();
          $("#blazemeter-signup").hide();
          $("#blazemeter-login").hide();
          $("#edit-meta-userkey").val("user key is stored");
          $("#edit-meta-userkey-holder").val("user key is stored");
        }
        else {
          //Login/registration is not finished yet, check again after 2 sec.
          window.setTimeout(function () {
            blazemeter_userkey_check();
          }, 2000);
        }
      }
    });
  }
});

