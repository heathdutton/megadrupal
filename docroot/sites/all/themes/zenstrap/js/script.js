/**
 * @file
 * A JavaScript file for the theme.
 */

(function ($, Drupal, window, document, undefined) {

Drupal.behaviors.zenstrap_vertical_tabs = {
  attach:function (context) {
    if ($('div.vertical-tabs').length > 0) {
      $('div.vertical-tabs').addClass('tabbable').addClass('tabs-left').removeClass('vertical-tabs');
      $('ul.vertical-tabs-list').addClass('nav').addClass('nav-tabs').removeClass('vertical-tabs-list');
      $('div.vertical-tabs-panes').addClass('tab-content').removeClass('vertical-tabs-panes');
      $('li.vertical-tab-button.selected').addClass('active').removeClass('selected');
      $('li.vertical-tab-button').each(function(){$(this).removeClass('vertical-tab-button');});
      $('div.tabbable ul a').each(function(){$(this).attr('data-toggle', 'tab');});
      $('div.tabbable a span').each(function(){$(this).addClass('nav-header');});
      $('div.tabbable ul li a').bind('click', function(){
        $('div.tabbable ul li.active').removeClass('active');
        $(this).parent().addClass('active').removeClass('selected');
      });
    }
    
  }
}

Drupal.behaviors.zenstrap_buttons = {
  attach:function (context) {
    if ($('.form-actions').length > 0) {
      $('.form-actions input:first').addClass('btn-primary');
    }
  }  
}

Drupal.behaviors.zenstrap_login_modal = {
  attach:function (context) {
    if (Drupal.settings.zenstrap_login_modal) {
      $("[href*='user/login']").bind('click', function() {
        $('#login-modal').modal({keyboard: false});
        return false; 
      });
    }
  }  
}

Drupal.behaviors.zenstrap_forms_modal = {
  attach:function (context) {
    if (Drupal.settings.zenstrap_forms_modal) {
      for (var id in Drupal.settings.zenstrap_forms_modal) {
        var path = "[href*='" + Drupal.settings.zenstrap_forms_modal[id] + "']";
        $(path).live('click', function() {
          //hide the old content
          $('#content_modal').hide();
          //show the waiting
          $('#waiting_modal').show();
          //show the modal
          $('#forms-modal').modal({keyboard: false});
          formUrl = $(this).attr('href') + '?modal=TRUE';
          //fetch data and show
          $.get(formUrl, function(data){
            $('#content_modal').html(data);
            $('#waiting_modal').fadeOut(300, function(){
              $('#content_modal').fadeIn();
            }); 
          });            
          return false;
        });
      }
    }
  }  
}

Drupal.behaviors.zenstrap_form_inline = {
  attach:function (context) {
    if ($('form.form-inline').length > 0) {
      $('form.form-inline label').each(function(){
        $id = $(this).attr('for');
        $('#' + $id).attr('placeholder', $(this).text());
      });
    }
  }  
}

Drupal.behaviors.zenstrap_menus = {
  attach:function (context) {
    //expand the accordians
    if ($('.accordion-inner').length > 0) {
      $('.accordion-inner ul').addClass('nav nav-stacked nav-pills');
      $('.accordion-body').each(function(){
        if ($(this).find('.active').length > 0) {
          $(this).addClass('in');
        }
      });
    }
  }
}

Drupal.behaviors.zenstrap_jthemes_override = {
  attach:function (context) {
    Drupal.theme.prototype.tableDragChangedWarning = function () {
      var msg = '<div class="tabledrag-changed-warning messages alert">' +
        '<button class="close" data-dismiss="alert">×</button>' + 
        Drupal.theme('tableDragChangedMarker') + ' ' + Drupal.t('Changes made in this table will not be saved until the form is submitted.') + '</div>';
       return msg;
    };
  }
}
})(jQuery, Drupal, this, this.document);
