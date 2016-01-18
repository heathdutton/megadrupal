(function ($) {

  Drupal.behaviors.archibald_user_vcard = {
    attach: function () {
      archibald_hook_user_edit_vcard();
    }
  };

  /**
   * @var temporery object cache to know where to save the currently choosen contributor
   */
  var archibald_contributor_cur = {'role':'', 'name':'', 'entity':''};
  var name = "";

  function archibald_hook_user_edit_vcard() {
    name = $("#edit-field-user-vcard-und-0-value");
    $(name).hide();

    $("#edit-field-vcard-und-0-value-wrapper").remove();
    var inputWrapper = document.createElement("input");

    $(inputWrapper)
      .attr("id", "edit-field-vcard-und-0-value-wrapper")
      .attr("name", "edit-field-vcard-und-0-value-wrapper")
      .addClass("form-text")
      .css("width", "100%")
      .unbind('click', archibald_user_edit_vcard_action_handler)
      .bind('click', archibald_user_edit_vcard_action_handler)
      .after(inputWrapper);

    $(name).after(inputWrapper);

    var ajax_url = 'archibald_contributor/get/' + name.val();
    if (Drupal.settings.archibald.url_detection_enabled) {
      ajax_url = Drupal.settings.archibald.current_language + '/' + ajax_url;
    }

    if(name.val() != "") {
        $.ajax({
          type: 'POST',
          url: '/?q=' + ajax_url,
          success: function(response, status) {
            if(response != "") {
              $(inputWrapper).val(response.firstname + " " + response.lastname + " " + response.organisation);
            }
          },
          dataType: 'json'
        });
    }

    $('.form_contributor_add_cancel').unbind('click', archibald_contributor_add_form_cancel_handler);
    $('.form_contributor_add_cancel').bind('click', archibald_contributor_add_form_cancel_handler);

    $('.archibald_contributor_choose').unbind('click', archibald_contributor_choose_handle);
    $('.archibald_contributor_choose').bind('click', archibald_contributor_choose_handle);

    $('.archibald_contributor_reset').unbind('click', archibald_contributor_reset_handle);
    $('.archibald_contributor_reset').bind('click', archibald_contributor_reset_handle);

    // if we add a new one we will show the whole list afterwards and choose one with this tag
    $('.archibald_contributor_choose_this').each(archibald_contributor_choose_handle);

    $('.archibald_contriberter_page').each(function() {
      $(this).unbind('click', archibald_contributor_page_link_handle);
      $(this).bind('click', archibald_contributor_page_link_handle);
    });
  }

  /**
   * event handler for clicking on contribute name field
   */
  function archibald_user_edit_vcard_action_handler() {
    Drupal.CTools.Modal.show({ modalTheme: 'archibaldModalDialog' });

    var contributor = $(this).parents('.archibald_life_cycle_contribute_item');
    archibald_contributor_cur.name = $(this);
    archibald_contributor_cur.entity = $('edit-field-vcard-und-0-value',  contributor);

    var ajax_url = 'archibald_contributor/chooser/ajax';
    if (Drupal.settings.archibald.url_detection_enabled) {
        ajax_url = Drupal.settings.basePath + Drupal.settings.pathPrefix + ajax_url;
    }

    $.ajax({
      type: 'POST',
      url: ajax_url,
      data: {},
      success: function(response, status) {
        for (var i in response) {
          if (response[i]['command'] && Drupal.ajax.prototype.commands[response[i]['command']]) {
            Drupal.ajax.prototype.commands[response[i]['command']](Drupal.ajax, response[i], status);
          }
        }
      },
      dataType: 'json'
    });
  }

  /**
   * handler for add form cancel button
   */
  function archibald_contributor_add_form_cancel_handler() {
    $.ajax({
      type: 'POST',
      url: '/?q=archibald_contributor/chooser/ajax',
      success: function(response, status) {
        for (var i in response) {
          if (response[i]['command'] && Drupal.ajax.prototype.commands[response[i]['command']]) {
            Drupal.ajax.prototype.commands[response[i]['command']](Drupal.ajax, response[i], status);
          }
        }
      },
      dataType: 'json'
    });

    return false;
  }

  /**
   * handler for chosse contribute item
   */
  function archibald_contributor_choose_handle() {
    archibald_contributor_cur.entity.val($(this).attr('contributor_id'));
    archibald_contributor_cur.name.val($(".archibald_contributor_list_name").html());

    $("#edit-field-user-vcard-und-0-value").val($(this).attr('contributor_id'));
    $("#edit-field-vcard-und-0-value-wrapper").val($(".archibald_contributor_list_name").html());
    modalContentClose();
    return false;
  }

  /**
   * handler for reset contribute item
   */
  function archibald_contributor_reset_handle() {
    archibald_contributor_cur.entity.val("");
    archibald_contributor_cur.name.val("");
    modalContentClose();
    return false;
  }

  /**
   * handle contriber overlay pager
   */
  function archibald_contributor_page_link_handle() {
    $.ajax({
      type: 'GET',
      url: $(this).attr('href'),
      success: function(response) {
        $('#archibald_choose_contributor_search_result').html(response);
        Drupal.attachBehaviors('#modalContent .modal-content');
      },
      dataType: 'html'
    });

    return false;
  }

  /**
   * Provide the HTML to create the modal dialog.
   */
  Drupal.theme.prototype.archibaldModalDialog = function () {
    var html = ''
    html += '  <div id="ctools-modal">'
    html += '    <div class="ctools-modal-content">' // panels-modal-content
    html += '      <div class="modal-header">';
    html += '        <a class="close" href="#">';
    html +=            Drupal.t('Close Window') + ' <img src="' + Drupal.settings.archibald.urls.module_base_path + '/images/close.png" border="0" />';
    html += '        </a>';
    html += '        <span id="modal-title" class="modal-title">&nbsp;</span>';
    html += '      </div>';
    html += '      <div id="modal-content" class="modal-content">';
    html += '      </div>';
    html += '    </div>';
    html += '  </div>';

    return html;
  }

})(jQuery);
