/**
 * @file
 *
 * @author Dennis Brücke (blackice2999) | TWD - team:Werbedesign UG
 *   @see http://drupal.org/user/413429
 *   @see http://team-wd.de
 */

;
(function ($) {
  "use strict";

  Drupal.bootstrap_api = Drupal.bootstrap_api || {};
  Drupal.bootstrap_api.Modal = Drupal.bootstrap_api.Modal || {};

  // ************************************************************************************************
  // * Drupal.behavior
  // ************************************************************************************************
  Drupal.behaviors.bootstrap_api_modal = {
    attach: function (context, settings) {
      $('body', context).once('bootstrap-api-modal', function () {
        // @TODO: implement stacking modals
        Drupal.bootstrap_api.Modal[0] = new Drupal.bootstrap_api.Modal(this, {});
      });

      Drupal.bootstrap_api.Modal[0].scanButtons();
    }
  };

  // ************************************************************************************************
  // * Drupal.BootstrapModal Object
  // ************************************************************************************************

  Drupal.bootstrap_api.Modal = function (parentEl, element_settings) {

    var defaults = {
      backdrop: true,
      keyboard: true,
      show:     false,
      remote:   false
    };

    this.options = $.extend({}, defaults, element_settings);

    this.$el = $(Drupal.theme("bootstrap_api_modal")).modal(this.options);
    this.$titleEl = $('.modal-header h3', this.$el);
    this.$contentEl = $('.modal-body', this.$el);
    this.$footerEl = $('.modal-footer', this.$el);

    var modal = this;
    this.$el.bind('show', function (event) {
      modal.onShow(this, event);
    });
    this.$el.bind('shown', function (event) {
      modal.onShown(this, event);
    });
    this.$el.bind('hide', function (event) {
      modal.onHide(this, event);
    });
    this.$el.bind('hidden', function (event) {
      modal.onHidden(this, event);
    });

    $(parentEl).append(this.$el);

    return this;
  };

  // Methods

  Drupal.bootstrap_api.Modal.prototype.show = function (content) {
    var modal = this;
    modal.$el.modal('show');
  };

  Drupal.bootstrap_api.Modal.prototype.hide = function () {
    var modal = this;
    modal.$el.modal('hide');
  };

  Drupal.bootstrap_api.Modal.prototype.setTitle = function (title) {
    var modal = this;
    modal.$titleEl.html(title);
  };

  Drupal.bootstrap_api.Modal.prototype.clearTitle = function () {
    var modal = this;
    modal.$titleEl.empty();
  };

  Drupal.bootstrap_api.Modal.prototype.setAttributes = function (attributes) {
    var modal = this;

    modal.$el.css(attributes);
  };

  Drupal.bootstrap_api.Modal.prototype.resizeHeight = function () {
    var modal = this;
    var old_h = modal.$contentEl.css('max-height');
    this.old_h = old_h;
    old_h = old_h.substring(0, old_h.length - 2)
    var new_h = $(window).height() - 200;
    if (new_h < old_h) {
      modal.$contentEl.css('max-height', new_h + 'px');
    }
  };

  // Workaround for buttons...
  Drupal.bootstrap_api.Modal.prototype.scanButtons = function () {
    var modal = this;

    // get buttons from new content
    var new_buttons = $('<div></div>');

    // Scan for Buttons in content...
    $('[data-bootstrap-api-modal-button="1"]', modal.$contentEl).each(function () {
      var $targetEl = $(this);
      var targetTitle = $($targetEl).data('bootstrapApiModalTitle');

      if ($targetEl.hasClass('btn')) {
        var classesList = $targetEl.attr('class').split(/\s+/);
      }

      var newbtn = $('<a href="#"></a>').addClass(classesList.join(" ")).html(targetTitle).bind('click', function (e) {
        e.preventDefault();
        $($targetEl).trigger('mousedown');
        $(this).button('toggle').addClass('disabled');
      });

      new_buttons.append(newbtn);
    });

    modal.$footerEl.html(new_buttons);
  };

  // Events

  Drupal.bootstrap_api.Modal.prototype.onShow = function (element, event) {
    // placeholder event
    var modal = this;
    modal.resizeHeight();
  };

  Drupal.bootstrap_api.Modal.prototype.onShown = function (element, event) {
    // placeholder event
  };

  Drupal.bootstrap_api.Modal.prototype.onHide = function (element, event) {
    var modal = this;
    modal.$contentEl.css('max-height', this.old_h);
    Drupal.detachBehaviors(modal.$el);
    modal.$contentEl.empty(); // kill content to prevent duplicate form ids
  };

  Drupal.bootstrap_api.Modal.prototype.onHidden = function (element, event) {
    // placeholder event
  };

  // ************************************************************************************************
  // * Drupal.theme functions
  // ************************************************************************************************

  Drupal.theme.prototype.bootstrap_api_modal = function () {
    var html = '';

    html += '<div id="bootstrap-modal" class="modal hide fade">';
    html += '  <div class="modal-header">';
    html += '    <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>';
    html += '    <h3></h3>';
    html += '  </div>';
    html += '  <div class="modal-body">';
    html += '  </div>';
    html += '  <div class="modal-footer">';
    // html += '    <a href="#" class="btn">Close</a>';
    // html += '    <a href="#" class="btn btn-primary">Save changes</a>';
    html += '  </div>';
    html += '</div>';

    return html;
  };

  Drupal.theme.prototype.bootstrap_api_modal_title = function () {
    var html = '<h3></h3>';
    return html;
  };

  Drupal.theme.prototype.bootstrap_api_modal_button = function () {
    var html = '<a href="#" class="btn"></a>';
    return html;
  }

  // ************************************************************************************************
  // * Drupal.ajax Commands
  // ************************************************************************************************
  $(function () {
    Drupal.ajax.prototype.commands.bootstrap_api_modal_show = function (ajax, response, status) {
      var modal = Drupal.bootstrap_api.Modal[0]; // @TODO

      var new_content_wrapped = $('<div></div>').html(response.content);
      var new_content = new_content_wrapped.contents();
      // assign new content to .modal-body
      modal.$contentEl.html(new_content);
      modal.setTitle(response.options.title);
      if (response.options.cssattributes) {
        modal.setAttributes(response.options.cssattributes);
      }

      // get custom buttons from settings
      modal.scanButtons();

      // attach drupal behaviors to new content
      var settings = response.settings || ajax.settings || Drupal.settings;
      Drupal.attachBehaviors(new_content, settings);

      modal.show();
    };

    Drupal.ajax.prototype.commands.bootstrap_api_modal_hide = function (ajax, response, status) {
      var modal = Drupal.bootstrap_api.Modal[0]; // @TODO
      modal.hide();
    };
  });

})(jQuery);
