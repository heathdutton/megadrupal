(function ($) {
  Drupal.TBRSSFeed = Drupal.TBRSSFeed || {};

  Drupal.TBRSSFeed.addAccount = function() {
    var accounts = $('#edit-tb-rss-feed-add-account-wrapper').find('fieldset');
    var account = accounts[0];
    var cloned_div = $(account).clone();
    //  cloned_div.find('input[name="' + service + '_feed_account[]"]').val('');
    cloned_div.find('input[type="text"]').val('');
    var timeStamp = new Date();
    timeStamp = timeStamp.getTime();
    cloned_div.attr('id', timeStamp);
    cloned_div.appendTo('#edit-tb-rss-feed-accounts-wrapper > .fieldset-wrapper');  
    return false;
  }
  Drupal.TBRSSFeed.deleteAccount = function(element) {
    $(element).parent().parent().remove();
    return false;
  }

  Drupal.TBRSSFeed.saveAndFeed = function() {
    $('input[name="tb_rss_feed_manually"]').val(1);
  }
  
  Drupal.TBRSSFeed.verifyAccount = function(url, el){
  var value = $(el).parent().find('input.account').val();
  var strUrl = url+value;
  window.open(strUrl, '_blank');
}

})(jQuery);
