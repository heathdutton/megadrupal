jQuery(function($){
$(".riveroftweets").tweet({
    avatar_size: Drupal.settings.riveroftweets.avatar,
    count: Drupal.settings.riveroftweets.count,
    username: [Drupal.settings.riveroftweets.username],
    template: Drupal.settings.riveroftweets.template,
    list: Drupal.settings.riveroftweets.list,
    query: Drupal.settings.riveroftweets.query,
    join_text:  'auto',
    auto_join_text_default: Drupal.settings.riveroftweets.joindefault,
    auto_join_text_ed: Drupal.settings.riveroftweets.joined,
    auto_join_text_ing: Drupal.settings.riveroftweets.joining,
    auto_join_text_reply: Drupal.settings.riveroftweets.joinreply,
    auto_join_text_url: Drupal.settings.riveroftweets.joinurl,
    refresh_interval: Drupal.settings.riveroftweets.refresh
});
})
