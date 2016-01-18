var archibald_modal_defaults = {
        modalTheme: 'archibaldModalDialog',
        modalSize:{
            type:'fixed',
            width:1250,
            height: (jQuery(window).height() * .9)
        }
    };

(function ($) {

if (typeof($(document).tooltip) == 'function') $( document ).tooltip();

Drupal.behaviors.archibald = {
  attach: function (context, settings) {
    archibald_identifier_entry_handler();
    archibald_contributor_handler();
    archibald_classification_handler();
    archibald_classification_display();
    archibald_image_preview_handler();
    archibald_keyword_handler();
    //archibald_spell_check_handler();
    archibald_details_button_handler();
    archibald_handle_edit_final_status();
    //archibald_handle_sortable();
    $(document.body).delegate(".archibald_remove_relation", "click", function() {archibald_remove_relation(this)}
    );
  }
};

function archibald_remove_relation(el){
  var p = $(el).parent();
  p.find('input[type=text].relation_value').val('');
  p.find('textarea.relation_description').val('');

  var pp = p.parent();
  var rels = pp.find('.archibald_relation');
  var totrels = 0;
  $.each( rels, function(){
    if( $(this).css('display') != 'none' ) totrels++;
  });

  if( totrels > 1 ) {
    p.fadeOut().css('display', 'none');
  }
}

/*
function archibald_handle_sortable() {
  $('#archibald_general_identifier').sortable({
    handle: 'span'
  });

  $('#archibald_general_identifier').unbind("sortstop").bind("sortstop", function(event, ui) {
    //var new_weights = $('#archibald_general_identifier').sortable('toArray');

    // Sort new weight.
    var new_weight = 0;
    $('.general_identifier_sort_weight').each(function() {
      $(this).val(new_weight++);
    });
  });

}*/

function archibald_handle_edit_final_status() {
  if(Drupal.settings.archibald_edit_final_status_open != undefined && Drupal.settings.archibald_edit_final_status_open != "") {

    var cancel_button = $('<input type="button" class="form-submit" value="' + Drupal.t('Cancel') + '"/>');
    var confirm_button = $('<input type="button" class="form-submit" value="' + Drupal.t('Confirm') + '"/>');

    var msg = '<div class="archibald_edit_final_field"><div>' + Drupal.t('Change status') + '</div>' + Drupal.t('The status of this description will change back to draft as soon as you edit and save it.') + '</div>';

    if (Drupal.settings.archibald_edit_final_status_is_published == 'published') {
      msg += '<div class="archibald_edit_final_field"><div>' + Drupal.t('Republish') + '</div>' + Drupal.t('The description must be republished after editing.') + '</div>';
    }

    var html  = '<div id="remove_archibald_handle_edit_final_status_dialog" title="' + Drupal.t('You are editing a description by the status final.') + '">';
        html += '   <div>' + msg + '</div>';
        html += '   <div id="remove_buttons"></div>';
        html += '</div>';

    $(html).dialog({
      modal: true,
      width: 500,
      height: 250,
      beforeClose: function() {
        return false;
      },
      open: function() {
        $(".ui-dialog-titlebar-close > span").html(Drupal.t('Close Window') + '<img border="0" src="' + Drupal.settings.archibald.urls.module_base_path + '/images/close.png">');
        $(".ui-dialog-titlebar-close").unbind('click').attr('href', Drupal.settings.archibald_edit_final_status_close);
        $("#remove_buttons").append(confirm_button);
        $("#remove_buttons").append(cancel_button);

        cancel_button.bind('click', function() {
          document.location.href = $(".ui-dialog-titlebar-close").attr('href');
        });

        confirm_button.bind('click', function() {
          $('#remove_archibald_handle_edit_final_status_dialog').dialog('destroy').remove();
        });
      }
    });

    Drupal.settings.archibald_edit_final_status_open = "";
  };
}

function archibald_keyword_handler() {
  $(".general_coverage:not(.processed)").each(function(a,b) {
      var elm = b;
      var delete_url = $("<a href='javascript:void(0);' class='delete_keyword'><span title='"+Drupal.t("Delete coverage")+"'></span></a>");
      $(elm).after(delete_url);
      $(elm).addClass("processed");
      $(elm).css("width", "90%");
      delete_url.click(function() {
          $(elm).attr("value", "");
          $(elm).parent().hide();
      });
  });

  $(".general_keyword:not(.processed)").each(function(a,b) {
      var delete_url = $("<a href='javascript:void(0);' class='delete_keyword'><span title='"+Drupal.t("Delete keyword")+"'></span></a>");
      $(this).after(delete_url);
      if ($(this).val() == 'archibald_delete') {
        $(this).val("");
      }
      $(this).addClass("processed");
      $(this).css("width", "90%");
      delete_url.click(function() {
          $('input', $(this).parent()).attr("value", "archibald_delete").parent().hide();
      });
      $(this).change(function() {
        var lang = $(this).attr('id').split("-", 3)[1];
        var is_unique = true;
        var value = $(this).val();
        var id = $(this).attr('id');
        if ($(this).val().trim() != "") {
          $('.keyword_' + lang + ':not(#' + id + ')').each(function() {
            if ($(this).val() == value) {
              is_unique = false;
            }
          });

          if (!is_unique) {
            alert(Drupal.t("Within this language the entered keyword already exists."));
            $(this).val("").focus();
          }
        }
      });
  });
}

/**
 * handle spell check checkbox visibilty
 *
 */
/*
function archibald_spell_check_handler() {
  $(".lang_string").each(function(a, b) {
      var spell_check_button = $('#'+$(b).attr('id')+'-spell-check').parent();

      $(b).parent().css('margin-bottom', '1.5em');
      if (spell_check_button.length>0) {
          spell_check_button.hide();

          $(b).focus(function () {
            spell_check_button.show('fast');
            $(b).parent().css('margin-bottom', '0px');
          });
      }
  });
}
*/
/**
 * switch to check if the page will loaded or we will vcalled by a ajax request
 * @var boolean
 */
var archibald_identifier_entry_handler_first_load = true;
/**
 * handle the form identifers
 */
function archibald_identifier_entry_handler() {
    var base_url = '/archibald_file';
    if (Drupal.settings.archibald && Drupal.settings.archibald.urls.archibald_file) {
        base_url = Drupal.settings.archibald.urls.archibald_file;
    }

    $('.archibald_general_identifier_item').each(function() {
     var identifier = $(this);
     var catalog  = $('.general_identifier_catalog',                     identifier);
     var entry    = $('.general_identifier_entity',                      identifier);
     var fid      = $('input[type=hidden][name$="\\[fid\\]\\[fid\\]"]',  identifier);
     var fid_int = parseInt(fid.val());

     var pattern = /archibald_file\/([0-9]+)\/.+/;
     var m = pattern.exec(entry.val());

     var old_fid = (m != null)?RegExp.$1:0;

     if (old_fid<=0 && fid_int>0) {
      // no url Item but a upload
      catalog.val('URL');
      entry.val('archibald_file/'+fid_int+'/upload_successfull');
      catalog.attr('readonly', 'readonly');
      entry.attr('readonly', 'readonly');

      $.ajax({
       url: base_url+'/'+fid_int+'/get_url',
       dataType: 'html',
       success: function(data) {
        // @TODO perhaps we can pre fill here some data of technical section
        entry.val(data);
       }
      });
     } else if (old_fid>0 && fid_int<=0) {
      if (archibald_identifier_entry_handler_first_load==true) {
       fid.val(old_fid);
      } else {
       entry.val('');
       catalog.attr('readonly', false);
       entry.attr('readonly', false);
      }
     }

     catalog.unbind('change');
     catalog.bind('change',   archibald_identifier_upload_display_handler);
     archibald_identifier_upload_display_handler(null, catalog);

     archibald_identifier_entry_handler_first_load = false;
    });
}

/**
 * identifer upload: handle if it will displayed or not
 * @param event
 * @param object  jQuery dom object
 */
function archibald_identifier_upload_display_handler(event, object) {
 var catalog = (object!=undefined)?object:$(this);
 var identifier = catalog.parents('.archibald_general_identifier_item');

 if (catalog.val()=='URL') {
  $('.tr_lower', identifier).show();
  $('.tr_upper_disabled', identifier).addClass('tr_upper').removeClass('tr_upper_disabled');
 } else {
  $('.tr_lower', identifier).hide();
  $('.tr_upper', identifier).addClass('tr_upper_disabled').removeClass('tr_upper');
 }
}

/**
 * @var temporery object cache to know where to save the currently choosen contributor
 */
var archibald_contributor_cur = {'role':'', 'name':'', 'entity':''};

/**
 * behavier handler for contributors
 */
function archibald_contributor_handler() {
    $('.archibald_life_cycle_contribute_item').each(function() {
     var name = $('.life_cycle_contribute_name', $(this));

     name.unbind('click');
     name.bind('click',   action_handler);
    });

    $('.form_contributor_add_cancel').unbind('click');
    $('.form_contributor_add_cancel').bind('click',   add_form_cancel_handler);

    $('.archibald_contributor_choose').unbind('click');
    $('.archibald_contributor_choose').bind('click',   choose_handle);

    $('.archibald_contributor_reset').unbind('click');
    $('.archibald_contributor_reset').bind('click',   reset_handle);

    // if we add a new one we will show the whole list afterwards and choose one with this tag
    $('.archibald_contributor_choose_this').each( choose_handle );

    $('.archibald_contriberter_page').each(function() {
     $(this).unbind('click');
     $(this).bind('click', page_link_handle);
    });

    // content add / edit form
    $('td.archibald_contributor_list_name[contributor_id]').unbind('mouseover');
    $('td.archibald_contributor_list_name[contributor_id]').bind('mouseover',   preview_handle);
    $('td.archibald_contributor_list_name[contributor_id]').unbind('mouseleave');
    $('td.archibald_contributor_list_name[contributor_id]').bind('mouseleave',   function () {$('#archibald_contributor_list_name_preview').hide();});
    $('#dsb-client-contributor-edit-form').unbind('mouseleave');
    $('#dsb-client-contributor-edit-form').bind('mouseleave',   function () {$('#archibald_contributor_list_name_preview').hide();});


    // content view
    $('.archibald_view_general .contributor[contributor_id]').unbind('mouseover');
    $('.archibald_view_general .contributor[contributor_id]').bind('mouseover',   display_vcard_handle);
    $('.archibald_view_general .contributor[contributor_id]').unbind('mouseleave');
    $('.archibald_view_general .contributor[contributor_id]').bind('mouseleave',   function () {$('#archibald_contributor_vcard').hide();});

    // change log
    $('span.archibald_hover_change_log[lom_id]').unbind('mouseover');
    $('span.archibald_hover_change_log[lom_id]').bind('mouseover',   display_change_handle);
    $('span.archibald_hover_change_log[lom_id]').unbind('mouseleave');
    $('span.archibald_hover_change_log[lom_id]').bind('mouseleave',   function () {$('#archibald_change_log_div').hide();});


    /**
     * event handler for clicking on contribute name field
     */
    function action_handler() {
      var archibald_modal_contributor_defaults = archibald_modal_defaults;
      archibald_modal_contributor_defaults.modalSize = {width: 1150, type:'fixed', height: (jQuery(window).height() * .9)};
      Drupal.CTools.Modal.show(archibald_modal_contributor_defaults);

      var contributor = $(this).parents('.archibald_life_cycle_contribute_item');
      archibald_contributor_cur.role = $('.life_cycle_contribute_role', contributor);
      archibald_contributor_cur.name = $(this);
      archibald_contributor_cur.entity = $('input[type=hidden][name$="\\[entity\\]"]', contributor);


      $.ajax({
        type: 'POST',
        url: Drupal.settings.archibald.urls.contributor_list,
        data: {entity:archibald_contributor_cur.entity.val(), self_lom_id:$('input[type=hidden][name=lom_id]').val()},
        success: archibald_ajax_response_handle,
        dataType: 'json'
      });
    }

    /**
     * handler for add form cancel button
     */
    function add_form_cancel_handler() {
          var query = $('.archibald_choose_contributor_query').val();
          var operation_type = $('.archibald_choose_contributor_form_operation_type').val();

          if (operation_type=='standalone') {
            Drupal.CTools.Modal.dismiss();
            return false;
          }

          $.ajax({
            type: 'POST',
            url: Drupal.settings.archibald.urls.contributor_list+'/'+query,
            success: archibald_ajax_response_handle,
            dataType: 'json'
          });

          return false;
    }

    /**
     * handler for chosse contribute item
     */
    function choose_handle() {
      if(archibald_contributor_cur.entity != undefined && archibald_contributor_cur.entity != '') {
        archibald_contributor_cur.entity.val(  $(this).attr('contributor_id') );
        archibald_contributor_cur.name.val(    $('.archibald_contributor_list_name', $(this).parents('tr')).text() );
        $('.archibald_contributor_vcards').hide();
        modalContentClose();
      }
      return false;
    }

    /**
     * handler for reset contribute item
     */
    function reset_handle() {
        archibald_contributor_cur.entity.val("");
        archibald_contributor_cur.name.val("");
        modalContentClose();
        return false;
    }

    /**
     * handle contriber overlay pager
     */
    function page_link_handle() {
     $.ajax({
        type: 'GET',
        url: $(this).attr('href').replace(/\/nojs(\/|$)/g, '/ajax$1'),
        success: function(response, status) {
         $('#archibald_choose_contributor_search_result').html(response);
         Drupal.attachBehaviors('#modalContent .modal-content');
        },
        dataType: 'html'
     });

     return false;
    }

    /**
     * display preview div
     **/
    var contributor_preview_handler_cache = new Object();
    function preview_handle(event) {

        var preview_div = $('#archibald_contributor_list_name_preview');
        if (preview_div.length<1) {
            preview_div = $('<div id="archibald_contributor_list_name_preview" class="archibald_contributor_vcards"><div id="content">&nbsp;</div></div>');
            $('body').append(preview_div);

            $('body').mousemove(function(event) {
               preview_div.css('left', (event.pageX+14)+'px');
               preview_div.css('top',  (event.pageY+14)+'px');
            });
        }

        /**
         * var offset = $(this).offset();
         * preview_div.css('left', (offset.left+14)+'px');
         * preview_div.css('top',  (offset.top+24)+'px');
         **/
        preview_div.css('margin-top', '0px');
        preview_div.show();
        calculate_preview_div_margin(preview_div);

        var contributor_id = $(this).attr('contributor_id');

        if (contributor_preview_handler_cache[contributor_id] != undefined ) {
            $('#content', preview_div).html(contributor_preview_handler_cache[contributor_id]);
            return true;
        }

        $('#content', preview_div).html('<div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div></div>');

        $.ajax({
            type: 'GET',
            url: Drupal.settings.archibald.urls.contributor_preview+'/'+contributor_id,
            success: function(response, status) {
                contributor_preview_handler_cache[contributor_id] = response;
                $('#content', preview_div).html(response);
            },
            dataType: 'html'
        });
    }

    /**
     * display preview div
     **/
    var vcard_cache = new Object();
    function display_vcard_handle(event) {
        var preview_div = $('#archibald_contributor_vcard');
        if (preview_div.length<1) {
            preview_div = $('<div id="archibald_contributor_vcard" class="archibald_contributor_vcards"><div id="content">&nbsp;</div></div>');
            $('body').append(preview_div);

            $('body').mousemove(function(event) {
               preview_div.css('left', (event.pageX+14)+'px');
               preview_div.css('top',  (event.pageY-60)+'px');
            });
        }

        var contributor_id = $(this).attr('contributor_id');
        preview_div.css('margin-top', '0px');
        preview_div.show();
        calculate_preview_div_margin(preview_div);

        if (vcard_cache[contributor_id] && vcard_cache[contributor_id].length>0) {
            $('#content', preview_div).html(vcard_cache[contributor_id]);
            return true;
        }

        $('#content', preview_div).html('<div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div></div>');

        var url = Drupal.settings.archibald.urls.archibald_vcard;
        url += '/'+Drupal.settings.archibald.lom.lom_id;
        url += '/'+Drupal.settings.archibald.lom.version;
        url += '/'+contributor_id;
        url += '/ajax';

        $.ajax({
            type: 'GET',
            url: url,
            success: function(response, status) {
                vcard_cache[contributor_id] = response;
                $('#content', preview_div).html(response);
            },
            dataType: 'html'
        });

        return true;
    }

    /**
     * display preview div
     **/
    var changes_cache = new Object();
    function display_change_handle(event) {
        var preview_div = $('#archibald_change_log_div');
        if (preview_div.length<1) {
            preview_div = $('<div id="archibald_change_log_div" class="archibald_contributor_vcards"><div id="content">&nbsp;</div></div>');
            $('body').append(preview_div);

            $('body').mousemove(function(event) {
               preview_div.css('left', (event.pageX+14)+'px');
               preview_div.css('top',  (event.pageY-60)+'px');
            });
        }

        var lom_id = $(this).attr('lom_id');
        preview_div.css('margin-top', '0px');
        preview_div.show();
        calculate_preview_div_margin(preview_div);

        if (changes_cache[lom_id] && changes_cache[lom_id].length>0) {
           // $('#content', preview_div).html(changes_cache[lom_id]);
           // return true;
        }

        $('#content', preview_div).html('<div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div></div>');
        $.ajax({
            type: 'GET',
            url: Drupal.settings.archibald.urls.archibald_change_log + lom_id,
            success: function(response, status) {
                changes_cache[lom_id] = response;
                $('#content', preview_div).html(response);
            },
            dataType: 'html'
        });

        return true;
    }

    /**
     * calculate preview div margin with vison to window height
     * @param preview_div jqueryq object of preview_div
     **/
    function calculate_preview_div_margin(preview_div) {
        // determine if overlay is complete visible
        var preview_div_height  = preview_div.height();
        var offset              = preview_div.offset();
        var preview_div_top     = offset.top;
        var window_height       = $(window).height();
        var window_top          = $(window).scrollTop();
        var preview2window      = preview_div_top-window_top; // abstand vom preview div zur fenster oberseite
        var space_below_preview = window_height-preview2window-preview_div_height;

        if (window_height<=preview_div_height) {
           // preview div is heigher them window to show it
           preview_div.css('margin-top', '-'+(preview_div_top-window_top-10)+'px');
        } else if (space_below_preview<0) {
           // cursor is to deep on scrren to show preview div
           preview_div.css('margin-top', space_below_preview+'px');
        }
    }
}


var session_ident_global = "";
/**
 * handle preview images of manged_fiels wwith spezial sufix div form preview images
 */
function archibald_image_preview_handler() {
    var base_url = '/archibald_file_preview';
    if (Drupal.settings.archibald && Drupal.settings.archibald.urls.archibald_file_preview) {
        base_url = Drupal.settings.archibald.urls.archibald_file_preview;
    }

    $('div.form-type-managed-file').each(function () {
        var preview_div = $('div.archibald_preview_image', $(this));

        if (preview_div.length==1) {
            var form_fid = $('input[type=hidden][name$="\\[fid\\]"]', $(this));
            var img_fid  = $('img[fid]', preview_div);

            form_fid = (form_fid.length==1)?form_fid.val():0;
            img_fid = (img_fid.length==1)?img_fid.attr('fid'):0;

            if (form_fid==0 && img_fid!=0) {
                preview_div.html('&nbsp');
            } else if (form_fid!='' && form_fid!=img_fid) {
                $.ajax({
                    type: 'GET',
                    url: base_url+'/'+form_fid+'/'+preview_div.attr('style_name'),
                    success: function(response, status) {
                        preview_div.html(response);
                    },
                    dataType: 'html'
                });
            }
        }
    });
}

var machine_name_global = "";

/**
 * baic hanlder for all classification curriculums
 * advacned special stuff you will find in curriculum_$MACHINE_NAME$.js
 * here we handly handle, loading (see the block below this function), add, remove process.
 * But nothing within add modal frame
 */
function archibald_classification_handler() {
    var base_url = Drupal.settings.archibald.urls.classification;

    var session_ident = $('.classification_session_ident').val();
    session_ident_global = session_ident;
    $('.archibald_classification_add').each(function() {
        $(this).unbind('click');
        $(this).bind('click',   add_handle);
    });

    $('.archibald_classification_remove').each(function() {
        $(this).unbind('click');
        $(this).bind('click',   remove_handle);
    });

    $('#edit-education-context .form-checkbox').unbind('change');
    $('#edit-education-context .form-checkbox').bind('change', valid_for_handle);
    valid_for_handle();

    /**
     * will called with a click on the add button
     */
    function add_handle(event) {
        event.preventDefault();
        machine_name_global = $(this).attr('machine_name');
        Drupal.CTools.Modal.show(archibald_modal_defaults);
        $.ajax({
            type: 'POST',
            url: base_url+'/get_add_form',
            data: {
                   session_ident:session_ident,
                   machine_name:$(this).attr('machine_name')
                  },
            success: archibald_ajax_response_handle,
            dataType: 'json'
        });
    }

    /**
     * will called with a click on the remove button
     */
    function remove_handle(event) {
        $(".ui-dialog-content").dialog("close");

        event.preventDefault();

        var attributes = $(this).attr('href').split('|');
        $.ajax({
            type: 'POST',
            url: base_url+'/remove',
            data: {session_ident:session_ident,
                   machine_name:attributes[0],
                   key:attributes[1]
                  },
            success: function(response, status) {
                $('.archibald_classification[machine_name='+attributes[0]+'] .archibald_classification_entrys').html(response);
                if ($('.archibald_classification_dialog_classifications').size()>0) {
                  $('.archibald_classification_dialog_classifications').html(response);
                }
                archibald_classification_handler(base_url);
                archibald_classification_display();
            },
            dataType: 'html'
        });
    }

    // determine witch curriculum is activ using context checkoxes
    function valid_for_handle() {
        var activ_context = new Object;
        $('#edit-education-context .form-checkbox:checked').each(function () {
            var context = $(this).attr('value').split('|');
            activ_context[context[1]] = true;
        });

        $('.archibald_classification').each(function () {
            var is_valid_for = false;
            var valid_for_list = $(this).attr('valid_for').split(';');

            if (valid_for_list.length<1) {
                is_valid_for = true;
            } else {
                for(i in valid_for_list) {
                    if (valid_for_list[i] == 'all' ||  (activ_context[valid_for_list[i]] && activ_context[valid_for_list[i]] == true)) {
                        is_valid_for = true;
                    }
                }
            }

            if (is_valid_for == true) {
                $(this).show();
            } else {
                $(this).hide();
            }
        });
    }
}

/**
 * Display handler for curriculum items
 **/
function archibald_classification_display() {
    $('.curriculum_per .detail img').show();

    $('.curriculum_per .detail img').unbind('click');
    $('.curriculum_per .detail img').bind('click', per_details_overlay_handler);


    // place current detail_informations to overlay
    function per_details_overlay_handler() {
        var detail_informations = jQuery('.detail_informations', jQuery(this).parent()).clone();
        detail_informations.show();
        detail_informations.dialog({
            modal:true,
            autoOpen:true,
            width: 750
        });
        archibald_classification_handler();
    }



    var educa_disciplin_overlay = $('div.discipline_dialog');

    if (educa_disciplin_overlay.length<1) {
        educa_disciplin_overlay = $('<div>');
        educa_disciplin_overlay.addClass('discipline_dialog');
        educa_disciplin_overlay.append('<div class="title">'+Drupal.t('Discipline')+'</div>');

        educa_disciplin_overlay.append('<div class="content">&nbsp;</div>');
        educa_disciplin_overlay.hide();
        $('body').append(educa_disciplin_overlay);

        $('body').mousemove(function(event) {
            educa_disciplin_overlay.css('left', (event.pageX-314)+'px');
            educa_disciplin_overlay.css('top',  (event.pageY-40)+'px');
        });
    }

    $('.curriculum_educa .subject').unbind('mouseover');
    $('.curriculum_educa .subject').bind('mouseover', educa_disciplin_overlay_handler);

    $('.curriculum_educa .subject').unbind('mouseleave');
    $('.curriculum_educa .subject').bind('mouseleave',   function () {educa_disciplin_overlay.hide();});

    // place current competences_infos to overlay on movei t with mosue
    function educa_disciplin_overlay_handler() {
        $('.content', educa_disciplin_overlay).html(  $('.subject_path', $(this)).clone().show()  );

        educa_disciplin_overlay.show();
    }
}

/**
 * copy of Drupal.ajax.prototype.success witt littel changes
 * @param response object
 * @param status object
 */
function archibald_ajax_response_handle(response, status) {
    for (var i in response) {
        if (response[i]['command'] && Drupal.ajax.prototype.commands[response[i]['command']]) {
            Drupal.ajax.prototype.commands[response[i]['command']](Drupal.ajax, response[i], status);
        }
    }
}

/**
 * handle the more details accordion like opener.
 */
function archibald_details_button_handler() {
  $(".details .detailscontainer").not('.activ').hide();

  $(".details .btndetails").unbind('click');
  $(".details .btndetails").click(function() {
        if(jQuery.browser.msie == true && jQuery.browser.version == "7.0" ) {
            $(this).parents(".details").find(".detailscontainer").toggle()
            .siblings(".detailscontainer:visible").slideUp();
        } else {
            $(this).parents(".details").find(".detailscontainer").slideToggle()
            .siblings(".detailscontainer:visible").slideUp();
        }

		$(this).toggleClass("inactive").siblings(".details .btndetails").removeClass("inactive");

        if ($(this).hasClass('remember_choice')) {
            var new_status = ($(this).hasClass('inactive'))?'0':'1';
            var ident = $(this).attr('id');
            if (ident) {
                $.cookie('btndetails_remember_choice_'+ident, new_status, {expires: 7, path: '/'});
            }
        }
	});

  // check if there is a cookie and open it
  $(".details .btndetails").each( function () {
    if ($(this).hasClass('remember_choice')) {
        var ident = $(this).attr('id');
        if (ident) {
            if ($.cookie('btndetails_remember_choice_'+ident)=='1') {
               $(this).parents(".details").find(".detailscontainer").addClass('activ').show();
               $(this).removeClass("inactive");
            }
        }
    }
   });
}

if (typeof Drupal.ajax == 'function') {
    Drupal.ajax.prototype.commands.archibald_ajax_update_classifications = function() {
        $.ajax({
            type: 'POST',
            url: Drupal.settings.archibald.urls.classification+'/get/edit',
            data: {
                session_ident:session_ident_global,
                machine_name:machine_name_global,
                without_delete: false
            },
            success: function(response, status) {
                $('.archibald_classification_dialog_classifications').html(response);
                archibald_classification_display();
                archibald_classification_handler();
            },
            dataType: 'html'
        });
    };

    Drupal.ajax.prototype.commands.archibald_ajax_init_classifications = function() {
      if(machine_name_global != "") {
          var curriculum_div = $("<div class='archibald_classification_dialog_classifications'>" + Drupal.t('Please wait. Loading ....') + "</div>");
          $(".archibald_classification_ajax_form").after(curriculum_div);
          Drupal.ajax.prototype.commands.archibald_ajax_update_classifications();
      }
    };

    Drupal.ajax.prototype.commands.archibald_contributor_preview_hide = function () {
      jQuery('#archibald_contributor_list_name_preview').hide();
    };
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

function archibald_modal_frame_heigh_auto() {
    if (jQuery('div.ctools-modal-content').length<1) {
        return false;
    }

    jQuery('div#modal-content').css('height', 'auto');
    jQuery('div.ctools-modal-content').css('height', 'auto');

    return true;
}

