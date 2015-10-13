/**
 * @file
 * JavaScript to allow the user to toggle the display of election posts, by
 * whether they are 'votable' (i.e. the user has access to vote).
 */
(function ($, Drupal) {

  Drupal.behaviors.election_vote_filter_posts = {

    attach: function (context, settings) {
      $('.view-election-posts').once('election-vote-filter-posts', function () {
        var list = $(this),
          posts = list.find('.election-post'),
          posts_votable = posts.filter('.election-post-votable'),
          posts_not_votable = posts.not('.election-post-votable');
        // Only act if there is at least 1 post with voting access denied, and
        // at least 1 with voting access allowed.
        if (!posts_not_votable.length || !posts_votable.length) {
          return;
        }
        // Create a toggle link.
        var toggle = $('<div>')
            .addClass('election-vote-filter-posts')
            .append('<strong>' + Drupal.t('Display: ') + '</strong>'
              + '<a class="election-vote-show-votable">' + Drupal.t('Votable') + '</a>'
              + ' | '
              + '<a class="election-vote-show-unvotable">' + Drupal.t('Unvotable') + '</a>'
              + ' | '
              + '<a class="election-vote-show-all">' + Drupal.t('All') + '</a>');
        // Add the toggle link above the list of posts.
        list.prepend(toggle);
        // Define toggle actions.
        var links = list.find('.election-vote-filter-posts a'),
          linkShowAll = links.filter('.election-vote-show-all'),
          linkShowVotable = links.filter('.election-vote-show-votable'),
          linkShowUnvotable = links.filter('.election-vote-show-unvotable'),
          showAll = function () {
            posts.show();
            links.removeClass('active');
            linkShowAll.addClass('active');
          },
          showVotable = function () {
            posts_not_votable.hide();
            posts_votable.show();
            links.removeClass('active');
            linkShowVotable.addClass('active');
          },
          showUnvotable = function () {
            posts_not_votable.show();
            posts_votable.hide();
            links.removeClass('active');
            linkShowUnvotable.addClass('active');
          };
        // Bind click events to the toggle links.
        linkShowAll.click(showAll);
        linkShowVotable.click(showVotable);
        linkShowUnvotable.click(showUnvotable);
        // Initially only show 'votable' posts.
        showVotable();
      });

    }

  };

}(jQuery, Drupal));
