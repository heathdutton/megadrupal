/*global Drupal: false */
(function ($, undefined) {

var first_downgrade = true;

var comment_timer_update = function(node, time_left) {
  if(time_left > 0) {
    /* update counter */
    setTimeout(function() {
      time_left -= 1;
      if(seconds_to_unit(time_left, node)==1){
        node = donwgrade_unit(node);
      }
      node.countdown.html(seconds_to_unit(time_left, node));
      comment_timer_update(node, time_left);
    }, 1000);
  }

  else {
    /* Destroy counter; Remove the edit link from the DOM */
    node.el.remove();
  }
};

var timer_update_message = function(node, time_left) {
  if(time_left > 0) {
    /* update counter */
    setTimeout(function() {
      time_left -= 1;
      if(seconds_to_unit(time_left, node)==1){
        node = donwgrade_unit(node);
      }
      node.countdown.html(seconds_to_unit(time_left, node));
      timer_update_message(node, time_left);
    }, 1000);
  }

  else {
    /* destroy counter */
    if(node.el.html().indexOf(Drupal.t('left to edit this'))!=-1){
      node.el
        .removeClass('status')
        .html(Drupal.t('This can no longer be edited.'))
        .addClass('error')
      ;
    }
  }
};

var donwgrade_unit = function(node) {
  switch (node.unit.text().substring(0,3)) {
    case 'min':
      node.unit.text(Drupal.t('seconds'));
      node.countdown.text('60');
      break;
    case 'hou':
      node.unit.text(Drupal.t('minutes'));
      node.countdown.text('60');
      break;
    case 'day':
      node.unit.text(Drupal.t('hours'));
      node.countdown.text('24');
      break;
  }
  return node;
}


var seconds_to_unit = function(time, node) {
  switch (node.unit.text().substring(0,3)) {
    case 'min':
      time = Math.ceil(time / 60);
      break;
    case 'hou':
      time = Math.ceil(time / 60 / 60);
      break;
    case 'day':
      time = Math.ceil(time / 60 / 60 / 24);
      break;
  }

  return time;
}

var set_time_unit = function(el) {
  try{
    var time_unit = $(el).find('span[class*="edit-limit-time-unit-"]').attr('class').replace(/edit-limit-time-unit-/, '');
    $(el).find('em.placeholder').first().next().text(Drupal.t(time_unit));
  }
  catch(e){}
}

Drupal.behaviors.tableSelect = {
  attach : function(context) {
    $("li.comment-edit a").each(function() {
      set_time_unit($(this));
      var $this = $(this), node = {
        'el' : $this,
        'countdown' : $this.find('em.placeholder').first(),
        'unit' : $this.find('em.placeholder').first().next()
      }, time_left = parseInt(node.countdown.text());
      if(seconds_to_unit(time_left, node)==1){
        node = donwgrade_unit(node);
      }
      node.countdown.html(seconds_to_unit(time_left, node));
      comment_timer_update(node, time_left);
    });
    /* initialize countdown for the message area */
    $(".messages").each(function() {
      set_time_unit($(this));
      var $this = $(this), node = {
        'el' : $this,
        'countdown' : $this.find('em.placeholder').first(),
        'unit' : $this.find('em.placeholder').first().next()
      }, time_left = parseInt(node.countdown.text());
      if(seconds_to_unit(time_left, node)==1){
        node = donwgrade_unit(node);
      }
      if(node.el.html().indexOf(Drupal.t("left to edit this"))!=-1)
        node.countdown.html(seconds_to_unit(time_left, node));
      timer_update_message(node, time_left);
    });
  }
};

}(jQuery));
