
/**
 * @file
 * easy_blog.js
 * js behaviors for easy blog tree archive.
 */

(function ($) {

    Drupal.behaviors.easyBlog = {
        attach: function (context, settings) {

            var parents = $('.easy-blog-archive .btn-arrow.active, .easy-blog-archive a.active', context).parents("ul:not(.easy-blog-archive-list)");

            if (parents.length) {
                parents.addClass('active');
                parents.parent().find(".btn-arrow:first").addClass('active');
            } else {
                var first = $('.easy-blog-archive ul:not(.easy-blog-archive-list):first', context);
                first.addClass('active');
                first.find(".btn-arrow:first").addClass('active');
                first.find("ul:first").addClass('active');
                $('.easy-blog-archive ul.easy-blog-archive-list', context).find(".btn-arrow:first").addClass('active');
            }

            $('.easy-blog-archive', context).find(".btn-arrow").click(function (e) {
                $(this).toggleClass('active');
                $(e.target).nextAll('ul').toggleClass('active');
            });
        },

        completedCallback: function () {
            // Do nothing. But it's here in case other modules/themes want to override it.
        }

    }
})(jQuery);