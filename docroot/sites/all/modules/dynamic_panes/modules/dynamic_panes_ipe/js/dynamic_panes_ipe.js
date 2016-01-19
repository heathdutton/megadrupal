(function($) {

  Drupal.dynamicPanesIpe = Drupal.dynamicPanesIpe || {};

  var sortableCache = [];
  var changed = false;

  Drupal.behaviors.dynamicPanesIpe = {
    attach: function (context) {
      $('body:not(.dynamic-panes-ipe)', context).addClass('dynamic-panes-ipe');

      $(window).bind('beforeunload', function () {
        if (changed) {
          return Drupal.t('This will discard all unsaved changes. Are you sure?');
        }
      });

      Drupal.dynamicPanesIpe.initSorting(context);
      Drupal.dynamicPanesIpe.disableSortable(context);

      if (Drupal.dynamicPanesIpe.getEditMode() == 'on') {
        Drupal.dynamicPanesIpe.hideControls(context);
        Drupal.dynamicPanesIpe.showForm(context);
        Drupal.dynamicPanesIpe.enableSortable(context);
      }

      $('#dynamic-panes-ipe-customize-page', context).click(function () {
        Drupal.dynamicPanesIpe.setEditMode('on');
        Drupal.dynamicPanesIpe.hideControls(context);
        Drupal.dynamicPanesIpe.showForm(context);
        Drupal.dynamicPanesIpe.enableSortable(context);
        return false;
      });

      $('#dynamic-panes-ipe-cancel', context).click(function () {
        if (!changed || confirm(Drupal.t('This will discard all unsaved changes. Are you sure?'))) {
          Drupal.dynamicPanesIpe.setEditMode('off');
          Drupal.dynamicPanesIpe.showControls(context);
          Drupal.dynamicPanesIpe.hideForm(context);
          Drupal.dynamicPanesIpe.disableSortable(context);
        }

        // Cancel the submission.
        return false;
      });
    }
  };

  /**
   * Show IPE toolbar controls.
   *
   * @param context
   *   An element to attach behaviors to. If none is given, the document element
   *   is used.
   */
  Drupal.dynamicPanesIpe.showControls = function (context) {
    $('div.dynamic-panes-ipe-button-container').show();
    $('div.dynamic-panes-ipe-on', context).stop(true, true).slideToggle(100);
    $('body', context).toggleClass('dynamic-panes-ipe-show-controls');
  };

  /**
   * Hide IPE toolbar controls.
   *
   * @param context
   *   An element to attach behaviors to. If none is given, the document element
   *   is used.
   */
  Drupal.dynamicPanesIpe.hideControls = function (context) {
    $('div.dynamic-panes-ipe-button-container').hide();
    $('div.dynamic-panes-ipe-on', context).stop(true, true).slideToggle(100);
    $('body', context).toggleClass('dynamic-panes-ipe-show-controls');
  };

  /**
   * Show IPE edit form.
   *
   * @param context
   *   An element to attach behaviors to. If none is given, the document element
   *   is used.
   */
  Drupal.dynamicPanesIpe.showForm = function (context) {
    $('#dynamic-panes-ipe-sort-form', context).show();
  };

  /**
   * Hide IPE edit form.
   *
   * @param context
   *   An element to attach behaviors to. If none is given, the document element
   *   is used.
   */
  Drupal.dynamicPanesIpe.hideForm = function (context) {
    $('#dynamic-panes-ipe-sort-form', context).hide();
  };

  /**
   * Init the sortable in IPE.
   *
   * @param context
   *   An element to attach behaviors to. If none is given, the document element
   *   is used.
   */
  Drupal.dynamicPanesIpe.initSorting = function (context) {
    $('div.dynamic-panes-ipe-sort-container', context).each(function (index, anchor) {
      $(this).sortable({
        handle: 'div.dynamic-panes-ipe-draghandle',
        opacity: 0.75
      });

      $(this).bind('sortupdate', function () {
        $(this).addClass('sortable-changed');
        changed = true;
      });

      sortableCache.push(anchor.innerHTML);
    });
  };

  /**
   * Enables the sortable in IPE.
   *
   * @param context
   *   An element to attach behaviors to. If none is given, the document element
   *   is used.
   */
  Drupal.dynamicPanesIpe.enableSortable = function (context) {
    $('div.dynamic-panes-ipe-sort-container', context).sortable('enable');
  };

  /**
   * Disable the sortable in IPE.
   *
   * @param context
   *   An element to attach behaviors to. If none is given, the document element
   *   is used.
   */
  Drupal.dynamicPanesIpe.disableSortable = function (context) {
    changed = false;
    $('div.dynamic-panes-ipe-sort-container', context).each(function (index, anchor) {
      if ($(this).hasClass('sortable-changed')) {
        anchor.innerHTML = sortableCache[index];
        Drupal.attachBehaviors($(anchor));
      }
    });

    $('div.dynamic-panes-ipe-sort-container', context).removeClass('sortable-changed').sortable('disable');
  };

  /**
   * Set edit mode to local storage.
   *
   * @param mode
   *   A new value of edit mode.
   */
  Drupal.dynamicPanesIpe.setEditMode = function (mode) {
    localStorage['editMode'] = mode;
  };

  /**
   * Get edit mode from local storage.
   *
   * @returns string
   *   The value of edit mode.
   */
  Drupal.dynamicPanesIpe.getEditMode = function () {
    var editMode = localStorage['editMode'];
    if (editMode) {
      return editMode;
    }

    return 'off';
  };

  /**
   * Apply new order of items.
   */
  Drupal.dynamicPanesIpe.applySort = function () {
    changed = false;
    $('div.dynamic-panes-ipe-sort-container.sortable-changed').each(function () {
      var val = '';
      $(this).find('div.dynamic-panes-ipe-block').each(function () {
        var blockId = $(this).data('block-id');
        if (blockId) {
          if (val) {
            val += ',';
          }
          val += blockId;
        }
      });

      var formElementName = $(this).data('form-element-name');
      $('[name="' +  formElementName + '"]', '#dynamic-panes-ipe-sort-form').val(val);
    });
  };

  var beforeSerialize = Drupal.ajax.prototype.beforeSerialize;

  /**
   * Handler for the form serialization.
   *
   * Runs before the beforeSend() handler (see below), and unlike that one, runs
   * before field data is collected.
   */
  Drupal.ajax.prototype.beforeSerialize = function (element, options) {
    beforeSerialize.call(this, element, options);
    if ($(this.element).hasClass('dynamic-panes-ipe-save')) {
      Drupal.dynamicPanesIpe.applySort();
    }
  };

})(jQuery);
