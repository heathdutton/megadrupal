/**
 * @file
 * Behaviours for the Shared Content UI.
 */

(function ($) {

    // Tab container counter.
    var sharedcontent_ui_tab_container_idx = 1;

    /**
     * Creates tabs in a container with the given boxes.
     *
     * @param container
     *   jQuery selector string of the container.
     * @param boxes
     *   Object with tab boxes and their names.
     *   Format:
     *   {
   *     jQuery selector of the box: name of the box
   *   }
     */
    var sharedcontent_ui_make_tabs = function (container, boxes) {
        var tab_container_id = 'tab_container_' + container.replace(/[\.#]/, '');
        var tab_container_selector = '#' + tab_container_id;

        // Create container if not already exists.
        if (!$(tab_container_selector).length) {
            $(container).prepend($('<ul class="tabs" id="' + tab_container_id + '"/>'));

            // Add clear div to clean floatness.
            $(container + ' ul').after('<div style="clear:both;"></div>');
        }
        var tab_idx = 0;
        var first_flag = true;

        // Generate tabs and process boxes.
        for (var idx in boxes) {
            // Don't do tab if missing container.
            if (!$(idx).length) continue;

            var panel_name = tab_container_id + '_panel_' + tab_idx++;
            var tab_name = tab_container_id + '_tab_' + tab_idx;
            var tab_id = '#' + tab_name;

            // Add tab if not already exists
            if (!$(tab_id).length) {
                $(tab_container_selector).append('<li id="' + tab_name + '" class="tab ' + (first_flag ? ' active' : '') + '"><a href="#' + panel_name + '"></a><span>f</span></li>');

                // Hide box.
                $(idx).attr('id', panel_name).hide();
                $(idx).addClass(tab_container_id);

                // Show first box.
                if (first_flag) {
                    $(idx).show();
                    first_flag = false;
                }
            }
            $(tab_id + ' span').html(boxes[idx]);
        }

        // Attach tab event.
        $(container + ' li.tab').bind('click', function () {
            // Inactivate tabs.
            $(container + ' li.tab').removeClass('active');
            // Activate current tab.
            $(this).addClass('active');
            // Hide boxes.
            $('.' + tab_container_id).hide();
            // Show current box.
            $($('a', this).attr('href')).show();

            // Store open tabs in cookie.
            sharedcontent_ui_create_cookie(tab_container_id, $('a', this).attr('href'));
        });

        // Keep tabs open as they were.
        var active_tab_href = sharedcontent_ui_get_cookie(tab_container_id);
        if (active_tab_href) {
            $(tab_container_selector + ' a[href=' + active_tab_href + ']').parent().click();
        }
    }


    Drupal.behaviors.sharedcontent_tabs = {
        attach:function (context, settings) {

            // Local Tabs.
            var local_queue_total = 0;
            var local_all_total = 0;

            // Check for @total in local headers.
            var local_queue = $('.view-id-sharedcontent_local_content.view-display-id-queue .total');
            var local_all = $('.view-id-sharedcontent_local_content.view-display-id-all .total');

            if (local_queue.text()) local_queue_total = local_queue.text();
            if (local_all.text()) local_all_total = local_all.text();

            // Create local tabs.
            sharedcontent_ui_make_tabs('.panel-col-first', {
                '.pane-sharedcontent-local-content-queue':Drupal.t('Queue (@value)', {'@value':local_queue_total}),
                '.pane-sharedcontent-local-content-all':Drupal.t('All (@value)', {'@value':local_all_total})
            });

            // Remove @total elements from local headers.
            local_queue.hide();
            local_all.hide();

            // Remote Tabs.
            var remote_queue_total = 0;
            var remote_all_total = 0;
            var remote_assignments_total = 0;
            var remote_suggestions_total = 0;
            var remote_linked_total = 0;

            // Check for @total in remote headers.
            var remote_queue = $('.view-id-sharedcontent_remote_content.view-display-id-queue .total');
            var remote_all = $('.view-id-sharedcontent_remote_content.view-display-id-all .total');
            var remote_assignments = $('.view-id-sharedcontent_remote_content.view-display-id-assignments .total');
            var remote_suggestions = $('.view-id-sharedcontent_remote_content.view-display-id-suggestions .total');
            var remote_linked = $('.view-id-sharedcontent_remote_content.view-display-id-linked .total');

            if (remote_queue.text()) remote_queue_total = remote_queue.text();
            if (remote_all.text()) remote_all_total = remote_all.text();
            if (remote_assignments.text()) remote_assignments_total = remote_assignments.text();
            if (remote_suggestions.text()) remote_suggestions_total = remote_suggestions.text();
            if (remote_linked.text()) remote_linked_total = remote_linked.text();

            // Create remote tabs.
            sharedcontent_ui_make_tabs('.panel-col-last', {
                '.pane-sharedcontent-remote-content-queue':Drupal.t('Queue (@value)', {'@value':remote_queue_total}),
                '.pane-sharedcontent-remote-content-all':Drupal.t('All (@value)', {'@value':remote_all_total}),
                '.pane-sharedcontent-remote-content-assignments':Drupal.t('Remotly assigned (@value)', {'@value':remote_assignments_total}),
                '.pane-sharedcontent-remote-content-suggestions':Drupal.t('Suggestions (@value)', {'@value':remote_suggestions_total}),
                '.pane-sharedcontent-remote-content-linked':Drupal.t('Linked (@value)', {'@value':remote_linked_total})
            });

            // Remove @total elements from remote headers.
            remote_queue.hide();
            remote_all.hide();
            remote_assignments.hide();
            remote_suggestions.hide();
            remote_linked.hide();

        }
    }
})(jQuery);

/**
 * Create a cookie.
 *
 * @param name
 *   Name string.
 * @param value
 *   Value string.
 * @param seconds
 *   Expire time integer in seconds.
 *   Leave it empty for session invalidation.
 */
function sharedcontent_ui_create_cookie(name, value, seconds) {
    var expire = '';
    if (seconds) {
        var date = new Date();
        date.setTime(date.getTime() + 1000 * seconds);
        expire = '; expires=' + date.toGMTString();
    }

    document.cookie = name + '=' + value + expire + '; path=' + Drupal.settings.basePath;
}

/**
 * Retrieve cookie.
 *
 * @param name
 *   Name string.
 * @return string
 */
function sharedcontent_ui_get_cookie(name) {
    var name_prefix = name + "=";
    var cookies = document.cookie.split(';');

    for (var i = 0; i < cookies.length; i++) {
        var cookie = cookies[i];
        while (cookie.charAt(0) == ' ') {
            cookie = cookie.substring(1, cookie.length);
        }

        if (cookie.indexOf(name_prefix) == 0) {
            return cookie.substring(name_prefix.length, cookie.length);
        }
    }

    return null;
}

/**
 * Erase cookie.
 *
 * @param name
 *   Name string.
 */
function sharedcontent_ui_remove_cookie(name) {
    sharedcontent_ui_create_cookie(name, '', -1);
}
