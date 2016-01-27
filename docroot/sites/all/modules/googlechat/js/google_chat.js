var windowFocus = true;
var username;
var lastStatus = Drupal.settings.googlechat.userStatus;
var chatHeartbeatCount = 0;
var minChatHeartbeat = 1000;
var maxChatHeartbeat = 33000;
var chatHeartbeatTime = minChatHeartbeat;
var originalTitle;
var blinkOrder = 0;

var chatboxFocus = new Array();
var newMessages = new Array();
var newMessagesWin = new Array();
var chatBoxes = new Array();
var emoticonImg;

jQuery(document).ready(function(){
  originalTitle = document.title;
  startChatSession();

  jQuery([window, document]).blur(function(){
    windowFocus = false;
  }).focus(function(){
    windowFocus = true;
    document.title = originalTitle;
  });

  jQuery('body').bind('mousemove keydown', function(){
    if(jQuery('#googlechat .status_indicator img:first-child').hasClass('googlechatstatus_3')) {
      updateChatStatus(lastStatus);
      chatHeartbeatTime = minChatHeartbeat;
      chatHeartbeatCount = 1;
    }
  });

  jQuery('body').delegate('div.google_chat_emoticons_rise', 'click', function(e){
    jQuery('div.google_chat_emoticons').css('display', 'none').removeClass('google_chat_emoticons_active');
    jQuery(this).children("div").css('display', 'block').addClass('google_chat_emoticons_active');
  });
  jQuery('body').delegate('div.google_chat_emoticons_active ul.J-KX-KY li', 'click', function(e){
    var currLI = jQuery(this);
    currLI.addClass('J-KX-KU-KO').siblings().removeClass('J-KX-KU-KO');
    currLI.parents().parents().find('.J-KX-Jv table').eq(currLI.index()).show().siblings().hide();
  });
  jQuery('body').bind('click', function(e){
    var clickedParent = jQuery(e.target).parent();
    if(jQuery(e.target).attr('class') != 'google_chat_emoticons_rise' &&
       !clickedParent.hasClass("google_chat_emoticons") &&
       !clickedParent.hasClass("J-KX-KU-KO") &&
       !clickedParent.hasClass("J-KX-KY") &&
       !clickedParent.hasClass("NQLkBe")) {
      jQuery('.google_chat_emoticons').hide().removeClass('google_chat_emoticons_active');
    }
  });
  jQuery('body').delegate('.J-JX-KA td', 'click', function(e){
    var currEle = jQuery(this);
    if(currEle.hasClass('J-JX-KA-JY')) {
      emoticonImg = currEle.children('div').attr('class');
    }
    else {
      emoticonImg = currEle.attr('class');
    }

    var selectedTxtArea = jQuery('.chatboxtextareaselected');
    switch (emoticonImg) {
      case 'k7':
      case 'k9':
      case 'k8':
      case 'k6':
        selectedTxtArea.val(selectedTxtArea.val() + " :) ");
        break;

      case 'j6':
      case 'j8':
      case 'j7':
      case 'j5':
        selectedTxtArea.val(selectedTxtArea.val() + " :D ");
        break;

      case 'lv':
      case 'lx':
      case 'lw':
      case 'lu':
        selectedTxtArea.val(selectedTxtArea.val() + " ;)");
        break;

      case 'jj':
      case 'jl':
      case 'jk':
      case 'ji':
        selectedTxtArea.val(selectedTxtArea.val() + " :'( ");
        break;

      case 'kZ':
      case 'k1':
      case 'k0':
      case 'kY':
        selectedTxtArea.val(selectedTxtArea.val() + " :-o ");
        break;

      case 'k3':
      case 'k5':
      case 'k4':
      case 'k2':
        selectedTxtArea.val(selectedTxtArea.val() + " :-/ ");
        break;

      case 'i0':
      case 'i2':
      case 'i1':
      case 'iZ':
        selectedTxtArea.val(selectedTxtArea.val() + " x-( ");
        break;

      case 'j2':
      case 'j4':
      case 'j3':
      case 'j1':
        selectedTxtArea.val(selectedTxtArea.val() + " :( ");
        break;

      case 'jf':
      case 'jh':
      case 'jg':
      case 'je':
        selectedTxtArea.val(selectedTxtArea.val() + " B-) ");
        break;

      case 'lr':
      case 'lt':
      case 'ls':
      case 'lq':
        selectedTxtArea.val(selectedTxtArea.val() + " :P ");
        break;

      case 'ka':
      case 'kc':
      case 'kb':
      case 'j9':
        selectedTxtArea.val(selectedTxtArea.val() + " <3 ");
        break;

      case 'lb':
      case 'ld':
      case 'lc':
      case 'la':
        selectedTxtArea.val(selectedTxtArea.val() + " :-| ");
        break;

    }
  });

  jQuery('#googlechat .status_indicator').bind('click mouseenter', function(){
    jQuery('#googlechat .googlechat_notification_sound_panel').hide();
    jQuery('#googlechat .status_indicator_panel').show();
  });
  jQuery('#googlechat .googlechat_setting_wrapper').bind('click mouseenter', function(){
    jQuery('#googlechat .status_indicator_panel').hide();
    jQuery('#googlechat .googlechat_notification_sound_panel').show();
  });
  jQuery('body').bind('click', function(e){
    var clickedParent = jQuery(e.target).parent();
    if(!clickedParent.hasClass("googlechat_subpanel_title") && !clickedParent.hasClass("status_indicator")) {
      jQuery('#googlechat .status_indicator_panel').hide();
    }
    if(!clickedParent.hasClass("sound_on") && !clickedParent.hasClass("sound_off")) {
      jQuery('#googlechat .googlechat_notification_sound_panel').hide();
    }
  });
  //googlechat panel status setting
  jQuery('#googlechat .status_indicator_panel ul.status li').bind('click', function(){
    var newStatus;
    var currentElement = jQuery(this);
    if (currentElement.index() == 0) {
      newStatus = Drupal.settings.googlechat.googlechatStatus.online;
    }
    else if (currentElement.index() == 1) {
      newStatus = Drupal.settings.googlechat.googlechatStatus.busy;
    }
    else if (currentElement.index() == 2) {
      newStatus = Drupal.settings.googlechat.googlechatStatus.offline;
    }
    updateChatStatus(newStatus);
  });
  //googlechat panel notification sound on/off
  jQuery('#googlechat .googlechat_notification_sound_panel ul li').bind('click', function(){
    var $this = jQuery(this);
    if ($this.hasClass('sound_on')) {
      jQuery.post(Drupal.settings.googlechat.changeSoundStatusURL, {sound_status: 1}, function(data){
        Drupal.settings.googlechat.userNotificationSound = data;
      });
    }
    else {
      jQuery.post(Drupal.settings.googlechat.changeSoundStatusURL, {sound_status: 2}, function(data){
        Drupal.settings.googlechat.userNotificationSound = data;
      });
    }
    jQuery('#googlechat .googlechat_notification_sound_panel').hide();
    $this.find('span').addClass('googlechat-icon-check').removeClass('clear_background_image');
    $this.siblings().find('span').addClass('clear_background_image').removeClass('googlechat-icon-check');
  });
  // googlechat panel minimize and maximize
  jQuery('#googlechat .googlechat_titlebar_min').bind('click', function(){
    var $this = jQuery(this);
    if($this.find('span').hasClass('googlechat-icon-minus')) {
      $this.find('span').removeClass('googlechat-icon-minus').addClass('googlechat-icon-newwin');
      jQuery('#googlechatbuddylist').hide();
      jQuery.cookie('googlechat_panel', 'no');
    }
    else {
      $this.find('span').addClass('googlechat-icon-minus').removeClass('googlechat-icon-newwin');
      jQuery('#googlechatbuddylist').show();
      jQuery.cookie('googlechat_panel', 'yes');
    }
  });
  if (jQuery.cookie('googlechat_panel') === 'yes') {
    jQuery('#googlechatbuddylist').css('display', 'block');
    jQuery('#googlechat .googlechat_titlebar_min').find('span').addClass('googlechat-icon-minus').removeClass('googlechat-icon-newwin');
  }
  else {
    jQuery('#googlechatbuddylist').css('display', 'none');
    jQuery('#googlechat .googlechat_titlebar_min').find('span').removeClass('googlechat-icon-minus').addClass('googlechat-icon-newwin');
  }
  jQuery('div.status_indicator,div.googlechat_setting_wrapper,div.googlechat_titlebar_min').hover(
    function(){
      jQuery(this).find('a').addClass('googlechat-state-hover');
      jQuery(this).find('span').addClass('googlechat-icon-hover').removeClass('googlechat-icon');
    },
    function(){
      jQuery(this).find('a').removeClass('googlechat-state-hover');
      jQuery(this).find('span').removeClass('googlechat-icon-hover').addClass('googlechat-icon');
    }
  );
  jQuery('div.googlechat_notification_sound_panel ul li').hover(
    function(){
      jQuery(this).addClass('googlechat-state-hover');
    },
    function(){
      jQuery(this).removeClass('googlechat-state-hover');
    }
  );
  // toggle chatbox setting
  jQuery('body').delegate('div.chatbox_setting_more', 'click mouseenter', function(e){
    jQuery("div.chatbox_setting_more_list").css('display', 'none');
    jQuery("div.chatbox_setting_more").removeClass('settingbackgroundFill');
    var ele = jQuery(this);
    ele.addClass('settingbackgroundFill');
    ele.siblings("div.chatbox_setting_more_list").toggle();
  });
  // closing chatbox setting list
  jQuery('body').bind('click', function(e){
    var clickedParent = jQuery(e.target).parent();console.log(clickedParent);
    if(!clickedParent.hasClass('chatbox_setting') && !clickedParent.hasClass("chatbox_setting_more") && !clickedParent.hasClass("chatbox_setting_more_list")) {
      jQuery("div.chatbox_setting_more_list").css('display', 'none');
      jQuery("div.chatbox_setting_more").removeClass('settingbackgroundFill');
    }
  });
  jQuery('body').delegate('div.chatbox_setting_more_list ul li', 'click', function(e){
    var url, toUser;
    var ele = jQuery(this);
    toUser = ele.parent().attr('class');
    if (ele.index() == 0) {
      url = Drupal.settings.googlechat.googlechatLog;
      $eleValue = ele.attr('value');
      jQuery.post(url, {chatlog: $eleValue,to: toUser}, function(data) {
        ele.attr('value', data.items[0].v);
        ele.text(data.items[0].t);
        var msg = '<div class="chatboxmessage"><span class="chatboxinfo" style="font-style: italic; font-size: 10px;margin-left:2em;">' + data.items[0].m + '</span></div>';
        jQuery("#chatbox_" + toUser + " .chatboxcontent").append(msg);
        jQuery("#chatbox_" + toUser + " .chatboxcontent").scrollTop(jQuery("#chatbox_" + toUser + " .chatboxcontent")[0].scrollHeight);
      });
    }
  });
});

function restructureChatBoxes() {
  align = 0;
  for (x in chatBoxes) {
    chatboxtitle = chatBoxes[x];

    if (jQuery("#chatbox_" + chatboxtitle).css('display') != 'none') {
      if (align == 0) {
        jQuery("#chatbox_" + chatboxtitle).css('right', '178px');
      } else {
        width = (align) * (225 + 7) + 154;
        jQuery("#chatbox_" + chatboxtitle).css('right', width + 'px');
      }
      align++;
    }
  }
}
function formatChatBoxTitle(title) {
  if (title.length > 0) {
    title = title.replace(/ /g, '_').replace(/\./g, '_').replace(/@/g, '_');
  }
  return title;
}

function chatWith(chatuser) {
  createChatBox(chatuser);
  jQuery("#chatbox_" + formatChatBoxTitle(chatuser) + " .chatboxtextarea").focus();
}

function createChatBox(chatboxtitle,  minimizeChatBox) {
  var newTitle = chatboxtitle;
  chatboxtitle = formatChatBoxTitle(chatboxtitle);
  if (jQuery("#chatbox_" + chatboxtitle).length > 0) {
    if (jQuery("#chatbox_" + chatboxtitle).css('display') == 'none') {
      jQuery("#chatbox_" + chatboxtitle).css('display','block');
      restructureChatBoxes();
    }
    jQuery("#chatbox_" + chatboxtitle + " .chatboxtextarea").focus();
    return;
  }
  var emoticonsHtml = '<div class="google_chat_emoticons" style="position:absolute;right: 2px; bottom: 0px;display:none;">';
  emoticonsHtml    += '<div class="J-KX J-KX-K9 NQLkBe">';
  emoticonsHtml    += '<ul class="J-KX-KY" tabindex="0">';
  emoticonsHtml    += '<li class="J-KX-KU-KO"><div class="lg"></div></li>';
  emoticonsHtml    += '<li><div class="lh"></div></li>';
  emoticonsHtml    += '<li><div class="li"></div></li>';
  emoticonsHtml    += '<li><div class="lj"></div></li>';
  emoticonsHtml    += '</ul>';
  emoticonsHtml    += '<div class="J-KX-Zf"></div>';
  emoticonsHtml    += '<div class="J-KX-Jv">';
  emoticonsHtml    += '<table class="J-JX-KA" tabindex="0" cellpadding="0" style="">';
  emoticonsHtml    += '<tbody>';
  emoticonsHtml    += '<tr class="J-JX-KA-axF">';
  emoticonsHtml    += '<td class="J-JX-KA-JY"><div class="k7" id=":35t.ic0" index="0"></div></td>';
  emoticonsHtml    += '<td class="J-JX-KA-JY"><div class="j6" id=":35t.ic1" index="1"></div></td>';
  emoticonsHtml    += '<td class="J-JX-KA-JY"><div class="lv" id=":35t.ic2" index="2"></div></td>';
  emoticonsHtml    += '<td class="J-JX-KA-JY"><div class="jj" id=":35t.ic3" index="3"></div></td>';
  emoticonsHtml    += '</tr>';
  emoticonsHtml    += '<tr class="J-JX-KA-axF">';
  emoticonsHtml    += '<td class="J-JX-KA-JY J-JX-KA-JU"><div class="kZ" id=":35t.ic4" index="4"></div></td>';
  emoticonsHtml    += '<td class="J-JX-KA-JY"><div class="k3" id=":35t.ic5" index="5"></div></td>';
  emoticonsHtml    += '<td class="J-JX-KA-JY"><div class="i0" id=":35t.ic6" index="6"></div></td>';
  emoticonsHtml    += '<td class="J-JX-KA-JY"><div class="j2" id=":35t.ic7" index="7"></div></td>';
  emoticonsHtml    += '</tr>';
  emoticonsHtml    += '<tr class="J-JX-KA-axF">';
  emoticonsHtml    += '<td class="J-JX-KA-JY"><div class="jf" id=":35t.ic8" index="8"></div></td>';
  emoticonsHtml    += '<td class="J-JX-KA-JY"><div class="lr" id=":35t.ic9" index="9"></div></td>';
  emoticonsHtml    += '<td class="J-JX-KA-JY"><div class="ka" id=":35t.ic10" index="10"></div></td>';
  emoticonsHtml    += '<td class="J-JX-KA-JY"><div class="lb" id=":35t.ic11" index="11"></div></td>';
  emoticonsHtml    += '</tr>';
  emoticonsHtml    += '</tbody>';
  emoticonsHtml    += '</table>';
  emoticonsHtml    += '<table class="J-JX-KA" tabindex="0" cellpadding="0" style="display: none;">';
  emoticonsHtml    += '<tbody>';
  emoticonsHtml    += '<tr class="J-JX-KA-axF">';
  emoticonsHtml    += '<td class="J-JX-KA-JY"><div class="k9" id=":35u.ic0" index="0"></div></td>';
  emoticonsHtml    += '<td class="J-JX-KA-JY"><div class="j8" id=":35u.ic1" index="1"></div></td>';
  emoticonsHtml    += '<td class="J-JX-KA-JY"><div class="lx" id=":35u.ic2" index="2"></div></td>';
  emoticonsHtml    += '<td class="J-JX-KA-JY"><div class="jl" id=":35u.ic3" index="3"></div></td>';
  emoticonsHtml    += '</tr>';
  emoticonsHtml    += '<tr class="J-JX-KA-axF">';
  emoticonsHtml    += '<td class="J-JX-KA-JY"><div class="k1" id=":35u.ic4" index="4"></div></td>';
  emoticonsHtml    += '<td class="J-JX-KA-JY"><div class="k5" id=":35u.ic5" index="5"></div></td>';
  emoticonsHtml    += '<td class="J-JX-KA-JY"><div class="i2" id=":35u.ic6" index="6"></div></td>';
  emoticonsHtml    += '<td class="J-JX-KA-JY"><div class="j4" id=":35u.ic7" index="7"></div></td>';
  emoticonsHtml    += '</tr>';
  emoticonsHtml    += '<tr class="J-JX-KA-axF">';
  emoticonsHtml    += '<td class="J-JX-KA-JY"><div class="jh" id=":35u.ic8" index="8"></div></td>';
  emoticonsHtml    += '<td class="J-JX-KA-JY"><div class="lt" id=":35u.ic9" index="9"></div></td>';
  emoticonsHtml    += '<td class="J-JX-KA-JY"><div class="kc" id=":35u.ic10" index="10"></div></td>';
  emoticonsHtml    += '<td class="J-JX-KA-JY"><div class="ld" id=":35u.ic11" index="11"></div></td>';
  emoticonsHtml    += '</tr>';
  emoticonsHtml    += '</tbody>';
  emoticonsHtml    += '</table>';
  emoticonsHtml    += '<table class="J-JX-KA" tabindex="0" cellpadding="0" style="display: none;">';
  emoticonsHtml    += '<tbody>';
  emoticonsHtml    += '<tr class="J-JX-KA-axF">';
  emoticonsHtml    += '<td class="J-JX-KA-JY"><div class="k8" id=":35v.ic0" index="0"></div></td>';
  emoticonsHtml    += '<td class="J-JX-KA-JY J-JX-KA-JU"><div class="j7" id=":35v.ic1" index="1"></div></td>';
  emoticonsHtml    += '<td class="J-JX-KA-JY"><div class="lw" id=":35v.ic2" index="2"></div></td>';
  emoticonsHtml    += '<td class="J-JX-KA-JY"><div class="jk" id=":35v.ic3" index="3"></div></td>';
  emoticonsHtml    += '</tr>';
  emoticonsHtml    += '<tr class="J-JX-KA-axF">';
  emoticonsHtml    += '<td class="J-JX-KA-JY"><div class="k0" id=":35v.ic4" index="4"></div></td>';
  emoticonsHtml    += '<td class="J-JX-KA-JY"><div class="k4" id=":35v.ic5" index="5"></div></td>';
  emoticonsHtml    += '<td class="J-JX-KA-JY"><div class="i1" id=":35v.ic6" index="6"></div></td>';
  emoticonsHtml    += '<td class="J-JX-KA-JY"><div class="j3" id=":35v.ic7" index="7"></div></td>';
  emoticonsHtml    += '</tr>';
  emoticonsHtml    += '<tr class="J-JX-KA-axF">';
  emoticonsHtml    += '<td class="J-JX-KA-JY"><div class="jg" id=":35v.ic8" index="8"></div></td>';
  emoticonsHtml    += '<td class="J-JX-KA-JY"><div class="ls" id=":35v.ic9" index="9"></div></td>';
  emoticonsHtml    += '<td class="J-JX-KA-JY"><div class="kb" id=":35v.ic10" index="10"></div></td>';
  emoticonsHtml    += '<td class="J-JX-KA-JY"><div class="lc" id=":35v.ic11" index="11"></div></td>';
  emoticonsHtml    += '</tr>';
  emoticonsHtml    += '</tbody>';
  emoticonsHtml    += '</table>';
  emoticonsHtml    += '<table class="J-JX-KA" tabindex="0" cellpadding="0" style="display: none;">';
  emoticonsHtml    += '<tbody>';
  emoticonsHtml    += '<tr class="J-JX-KA-axF">';
  emoticonsHtml    += '<td class="J-JX-KA-JY"><div class="k6" id=":35w.ic0" index="0"></div></td>';
  emoticonsHtml    += '<td class="J-JX-KA-JY"><div class="j5" id=":35w.ic1" index="1"></div></td>';
  emoticonsHtml    += '<td class="J-JX-KA-JY"><div class="lu" id=":35w.ic2" index="2"></div></td>';
  emoticonsHtml    += '<td class="J-JX-KA-JY"><div class="ji" id=":35w.ic3" index="3"></div></td>';
  emoticonsHtml    += '</tr>';
  emoticonsHtml    += '<tr class="J-JX-KA-axF">';
  emoticonsHtml    += '<td class="J-JX-KA-JY"><div class="kY" id=":35w.ic4" index="4"></div></td>';
  emoticonsHtml    += '<td class="J-JX-KA-JY"><div class="k2" id=":35w.ic5" index="5"></div></td>';
  emoticonsHtml    += '<td class="J-JX-KA-JY"><div class="iZ" id=":35w.ic6" index="6"></div></td>';
  emoticonsHtml    += '<td class="J-JX-KA-JY"><div class="j1" id=":35w.ic7" index="7"></div></td>';
  emoticonsHtml    += '</tr>';
  emoticonsHtml    += '<tr class="J-JX-KA-axF">';
  emoticonsHtml    += '<td class="J-JX-KA-JY"><div class="je" id=":35w.ic8" index="8"></div></td>';
  emoticonsHtml    += '<td class="J-JX-KA-JY"><div class="lq" id=":35w.ic9" index="9"></div></td>';
  emoticonsHtml    += '<td class="J-JX-KA-JY"><div class="j9" id=":35w.ic10" index="10"></div></td>';
  emoticonsHtml    += '<td class="J-JX-KA-JY"><div class="la" id=":35w.ic11" index="11"></div></td>';
  emoticonsHtml    += '</tr>';
  emoticonsHtml    += '</tbody>';
  emoticonsHtml    += '</table>';
  emoticonsHtml    += '</div>';
  emoticonsHtml    += '</div>';
  emoticonsHtml    += '</div>';
  var chattitletext = jQuery('#googlechatbuddy_' + chatboxtitle).attr('name');
  if (chattitletext == undefined) {
    chattitletext = newTitle;
  }
  var chatboxSetting = '<div class="chatbox_setting">';
  chatboxSetting += '<div class="chatbox_setting_more">';
  chatboxSetting += '&nbsp; More <span class="googlechat-icon-arrow-down googlechat-icon"></span>';
  chatboxSetting += '</div>';
  chatboxSetting += '<div class="chatbox_setting_more_list">';
  chatboxSetting += '<ul class="' + chatboxtitle + '">';
  chatboxSetting += '<li value="0">Go Off the Records</li>';
  /*chatboxSetting += '<li class="block">Block ' + chatboxtitle + '</li>';*/
  chatboxSetting += '</ul>';
  chatboxSetting += '</div>';
  chatboxSetting += '</div>';

  var chatboxHtml = '<div class="chatboxhead">';
  chatboxHtml    += '<div class="chatboxtitle">';
  chatboxHtml    += '<img src="' + Drupal.settings.googlechat.emoticonsURL + '" class="" />';
  chatboxHtml    += '<span>' + chattitletext + '</span></div>';
  chatboxHtml    += '<div class="chatboxoptions">';
  chatboxHtml    += '<a href="javascript:void(0)" onclick="javascript:toggleChatBoxGrowth(\'' + chatboxtitle + '\')">-</a>';
  chatboxHtml    += '<a href="javascript:void(0)" onclick="javascript:closeChatBox(\'' + chatboxtitle + '\')">X</a>';
  chatboxHtml    += '</div>';
  chatboxHtml    += '<br clear="all"/>';
  chatboxHtml    += '</div>';
  chatboxHtml    += chatboxSetting;
  chatboxHtml    += '<div class="chatboxcontent"></div>';
  chatboxHtml    += '<div class="googlechat_chatboxalert"></div>';
  chatboxHtml    += '<div class="chatboxinput">';
  chatboxHtml    += '<div class="google_chat_emoticons_rise">' + emoticonsHtml + '</div>';
  chatboxHtml    += '<textarea class="chatboxtextarea" onkeydown="javascript:return checkChatBoxInputKey(event,this,\'' + newTitle + '\');"></textarea>';
  chatboxHtml    += '</div>';

  jQuery(" <div />").attr("id","chatbox_" + chatboxtitle)
  .addClass("chatbox")
  .html(chatboxHtml)
  .appendTo(jQuery("body"));

  jQuery("#chatbox_" + chatboxtitle).css('bottom', '0px');

  chatBoxeslength = 0;

  for (x in chatBoxes) {
    if (jQuery("#chatbox_" + chatBoxes[x]).css('display') != 'none') {
      chatBoxeslength++;
    }
  }

  if (chatBoxeslength == 0) {
    jQuery("#chatbox_" + chatboxtitle).css('right', '154px');
  } else {
    width = (chatBoxeslength) * (225 + 7) + 154;
    jQuery("#chatbox_" + chatboxtitle).css('right', width + 'px');
  }

  chatBoxes.push(chatboxtitle);

  if (minimizeChatBox == 1) {
    minimizedChatBoxes = new Array();

    if (jQuery.cookie('chatbox_minimized')) {
      minimizedChatBoxes = jQuery.cookie('chatbox_minimized').split(/\|/);
    }
    minimize = 0;
    for (j = 0; j < minimizedChatBoxes.length; j++) {
      if (minimizedChatBoxes[j] == chatboxtitle) {
        minimize = 1;
      }
    }

    if (minimize == 1) {
      jQuery('#chatbox_' + chatboxtitle + ' .chatboxcontent').css('display','none');
      jQuery('#chatbox_' + chatboxtitle + ' .chatboxinput').css('display','none');
    }
  }

  chatboxFocus[chatboxtitle] = false;

  jQuery("#chatbox_" + chatboxtitle + " .chatboxtextarea").blur(function(){
    chatboxFocus[chatboxtitle] = false;
    jQuery("#chatbox_" + chatboxtitle + " .chatboxtextarea").removeClass('chatboxtextareaselected');
  }).focus(function(){
    chatboxFocus[chatboxtitle] = true;
    newMessages[chatboxtitle] = false;
    jQuery('#chatbox_' + chatboxtitle + ' .chatboxhead').removeClass('chatboxblink');
    jQuery("#chatbox_" + chatboxtitle + " .chatboxtextarea").addClass('chatboxtextareaselected');
  });

  jQuery("#chatbox_" + chatboxtitle).click(function() {
    if (jQuery('#chatbox_' + chatboxtitle + ' .chatboxcontent').css('display') != 'none') {
      jQuery("#chatbox_" + chatboxtitle + " .chatboxtextarea").focus();
    }
  });

  jQuery("#chatbox_" + chatboxtitle).show();
}


function chatHeartbeat(){

  var itemsfound = 0;
  if (windowFocus == false) {

    var blinkNumber = 0;
    var titleChanged = 0;
    for (x in newMessagesWin) {
      if (newMessagesWin[x] == true) {
        ++blinkNumber;
        if (blinkNumber >= blinkOrder) {
          document.title = x + ' says...';
          titleChanged = 1;
          break;
        }
      }
    }

    if (titleChanged == 0) {
      document.title = originalTitle;
      blinkOrder = 0;
    } else {
      ++blinkOrder;
    }

  } else {
    for (x in newMessagesWin) {
      newMessagesWin[x] = false;
    }
  }

  for (x in newMessages) {
    if (newMessages[x] == true) {
      if (chatboxFocus[x] == false) {
        //FIXME: add toggle all or none policy, otherwise it looks funny
        jQuery('#chatbox_' + x + ' .chatboxhead').toggleClass('chatboxblink');
      }
    }
  }
  // google chat heartbeat
  jQuery.ajax({
    url: Drupal.settings.googlechat.chatHeartBeatUrl,
    cache: false,
    dataType: "json",
    success: function(data) {

      jQuery.each(data.items, function(i, item){
        if (item != '{' && item != '}' && item.f != '') { /* fix strange ie bug*/

          chatboxtitle = formatChatBoxTitle(item.f);

          if (jQuery("#chatbox_" + chatboxtitle).length <= 0) {
            createChatBox(item.f);
          }
          if (jQuery("#chatbox_" + chatboxtitle).css('display') == 'none') {
            jQuery("#chatbox_" + chatboxtitle).css('display','block');
            restructureChatBoxes();
          }

          if (item.s == 1) {
            item.f = username;
          }
          item.m = linkify(item.m);
          msg = item.m.replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&quot;/g,"\"");
          if (item.s == 2) {
            jQuery("#chatbox_" + chatboxtitle + ">.chatboxcontent>.chatboxmessage:last-child>.chatboxusercontent").append('<span class="chatboxinfo sentat">' + msg + '</span>');
          } else {
            var chatFromName = jQuery('#googlechatbuddy_' + chatboxtitle).attr('title');
            if (chatFromName == undefined) {
              chatFromName = chatboxtitle;
            } else {
              chatFromName = chatFromName.substring(10);
            }
            newMessages[chatFromName] = true;
            newMessagesWin[chatFromName] = true;
            if (item.s == 1) {
              jQuery("#chatbox_" + chatboxtitle + " .chatboxcontent").append('<div class="chatboxmessage floatright"><div class="chatboxuserimage floatright"><img style="background: none repeat scroll 0px 0px rgb(0, 0, 0);" src="' + item.p + '" alt="' + item.r + '" title="' + item.r + '" width="30"/></div><div class="chatboxusercontent floatright"><span class="chatboxmessagecontent">' + msg + '</span></div><div class="msgtip tip_bottom"></div></div>');
            }
            else {
              jQuery("#chatbox_" + chatboxtitle + " .chatboxcontent").append('<div class="chatboxmessage"><div class="msgtip tip_top"></div><div class="chatboxuserimage"><img style="background: none repeat scroll 0px 0px rgb(0, 0, 0);" src="' + item.p + '" alt="' + item.r + '" title="' + item.r + '" width="30"/></div><div class="chatboxusercontent"><span class="chatboxmessagecontent">' + msg + '</span></div></div>');
            }

            // Play new message sound effect
            if (Drupal.settings.googlechat.notificationSound == 1 && Drupal.settings.googlechat.userNotificationSound == 1) {
              jQuery("#chatbox_" + chatboxtitle).append('<div id="googlechatbeep_wrapper_' + chatboxtitle + '" style="width: 0px; height: 0px; overflow: hidden;"><embed width="0" height="0" src="' + Drupal.settings.googlechat.sound + '" id="googlechatbeep" type="application/x-shockwave-flash"></div>');
              setTimeout(function () {jQuery('#googlechatbeep_wrapper_' + chatboxtitle).remove();}, 3000);
            }
          }

          jQuery("#chatbox_" + chatboxtitle + " .chatboxcontent").scrollTop(jQuery("#chatbox_" + chatboxtitle + " .chatboxcontent")[0].scrollHeight);
          itemsfound += 1;
        }
      });
      //update buddy status
      jQuery.each(data.openChatBoxStatus, function(name, value){
        if (name) {
          var chatboxalerthtml, chatboxname;
          chatboxname = formatChatBoxTitle(name);
          var chattitletext;
          if (value.realname == '') {
            chattitletext = name;
          } else {
            chattitletext = value.realname;
          }
          jQuery("#chatbox_" + chatboxname + " .chatboxtitle span").text(chattitletext);
          if (value.status == 2) {
            chatboxalerthtml = '<span class="busy">' + chattitletext + ' is busy. You may be interrupting.</span>';
            jQuery("#chatbox_" + chatboxname + " .googlechat_chatboxalert").html(chatboxalerthtml);
            jQuery("#chatbox_" + chatboxname + " .chatboxtitle img").attr("class", "googlechatstatus googlechatstatus_" + value.status);
            jQuery("#chatbox_" + chatboxname + " .chatbox_setting").css("border-bottom", "2px solid red");
          }
          else if (value.status == 4) {
            chatboxalerthtml = '<span class="offline">' + chattitletext + ' is offline. Messages you send will be delivered when ' + chattitletext + ' comes online.</span>';
            jQuery("#chatbox_" + chatboxname + " .googlechat_chatboxalert").html(chatboxalerthtml);
            jQuery("#chatbox_" + chatboxname + " .chatboxtitle img").attr("class", "googlechatstatus googlechatstatus_" + value.status);
            jQuery("#chatbox_" + chatboxname + " .chatbox_setting").css("border-bottom", "2px solid #5e5e5");
          }
          else if (value.status == 3) {
            jQuery("#chatbox_" + chatboxname + " .googlechat_chatboxalert").html("");
            jQuery("#chatbox_" + chatboxname + " .chatboxtitle img").attr("class", "googlechatstatus googlechatstatus_3");
            jQuery("#chatbox_" + chatboxname + " .chatbox_setting").css("border-bottom", "2px solid orange");
          }
          else {
            jQuery("#chatbox_" + chatboxname + " .googlechat_chatboxalert").html("");
            jQuery("#chatbox_" + chatboxname + " .chatboxtitle img").attr("class", "googlechatstatus googlechatstatus_1");
            jQuery("#chatbox_" + chatboxname + " .chatbox_setting").css("border-bottom", "2px solid #35AC1A");
          }
        }
      });
      //update buddy list
      jQuery("#googlechatbuddylist").html(data.buddylist);
      jQuery("#googlechat_panel_footer span").html(data.buddyListCount);
      chatHeartbeatCount++;

      if (itemsfound > 0) {
        chatHeartbeatTime = minChatHeartbeat;
        chatHeartbeatCount = 1;
      } else if (chatHeartbeatCount >= 10) {
        chatHeartbeatTime *= 2;
        chatHeartbeatCount = 1;
        if (chatHeartbeatTime > maxChatHeartbeat) {
          chatHeartbeatTime = maxChatHeartbeat;
          if(!jQuery('#googlechat .status_indicator img:first-child').hasClass('googlechatstatus_3')) {
            updateChatStatus(Drupal.settings.googlechat.googlechatStatus.away);
          }
        }
      }

      setTimeout('chatHeartbeat();', chatHeartbeatTime);
    }
  });
}
function updateChatStatus(newStatus) {
  if (newStatus != Drupal.settings.googlechat.googlechatStatus.away) {
    lastStatus = newStatus;
  }
  jQuery.ajaxSetup({async: false});
  jQuery.post(Drupal.settings.googlechat.statusUrl, {status: newStatus}, function(data){
    jQuery('#googlechat .status_indicator img').attr('class', 'googlechatstatus googlechatstatus_' + newStatus);
  });
  jQuery.ajaxSetup({async: true});
}
function closeChatBox(chatboxtitle) {
  jQuery('#chatbox_' + chatboxtitle).css('display','none');
  restructureChatBoxes();

  jQuery.post(Drupal.settings.googlechat.closeChatUrl, {
    chatbox: chatboxtitle
  } , function(data){
    });

}

function toggleChatBoxGrowth(chatboxtitle) {
  if (jQuery('#chatbox_' + chatboxtitle + ' .chatboxcontent').css('display') == 'none') {

    var minimizedChatBoxes = new Array();
    if (jQuery.cookie('chatbox_minimized')) {
      minimizedChatBoxes = jQuery.cookie('chatbox_minimized').split(/\|/);
    }

    var newCookie = '';
    for (i = 0; i < minimizedChatBoxes.length; i++) {
      if (minimizedChatBoxes[i] != chatboxtitle) {
        newCookie += minimizedChatBoxes[i] + '|';
      }
    }
    options = {};
    options.path = '/';
    newCookie = newCookie.slice(0, -1);
    jQuery.cookie('chatbox_minimized', newCookie, options);
    jQuery('#chatbox_' + chatboxtitle + ' .chatboxcontent').css('display','block');
    jQuery('#chatbox_' + chatboxtitle + ' .chatboxinput').css('display','block');
    jQuery("#chatbox_" + chatboxtitle + " .chatboxcontent").scrollTop(jQuery("#chatbox_" + chatboxtitle + " .chatboxcontent")[0].scrollHeight);
  }
  else {
    var newCookie = chatboxtitle;
    if (jQuery.cookie('chatbox_minimized')) {
      newCookie += '|' + jQuery.cookie('chatbox_minimized');
    }
    options = {};
    options.path = '/';
    jQuery.cookie('chatbox_minimized', newCookie, options);
    jQuery('#chatbox_' + chatboxtitle + ' .chatboxcontent').css('display','none');
    jQuery('#chatbox_' + chatboxtitle + ' .chatboxinput').css('display','none');
  }

}

function checkChatBoxInputKey(event,chatboxtextarea,chatboxtitle) {
  var smileSelecterIndex;
  if(event.keyCode == 13 && event.shiftKey == 0)  {
    message = jQuery(chatboxtextarea).val();
    message = message.replace(/^\s+|\s+jQuery/g,"");

    smileSelecterIndex = jQuery('.chatboxtextareaselected').parent().children().find('li.J-KX-KU-KO').index();

    jQuery(chatboxtextarea).val('');
    jQuery(chatboxtextarea).focus();
    jQuery(chatboxtextarea).css('height','44px');
    if (message != '') {
      message = gEmotions(smileSelecterIndex, message);
      message = message.replace(/</g,"&lt;").replace(/>/g,"&gt;").replace(/\"/g,"&quot;");
      jQuery.post(Drupal.settings.googlechat.sendChatUrl, {
        to: chatboxtitle,
        message: message
      } , function(data){
        message = data.items[0].m.replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&quot;/g,"\"");
        chatboxtitle = formatChatBoxTitle(chatboxtitle);
        jQuery("#chatbox_" + chatboxtitle + " .chatboxcontent").append('<div class="chatboxmessage floatright"><div class="chatboxuserimage floatright"><img style="background: none repeat scroll 0px 0px rgb(0, 0, 0);" src="' + data.items[0].p + '" alt="' + data.items[0].r + '" width="30"/></div><div class="chatboxusercontent floatright"><span class="chatboxmessagecontent">' + linkify(message) + '</span></div><div class="msgtip tip_bottom"></div></div>');
        jQuery("#chatbox_" + chatboxtitle + " .chatboxcontent").scrollTop(jQuery("#chatbox_" + chatboxtitle + " .chatboxcontent")[0].scrollHeight);
      });
    }
    chatHeartbeatTime = minChatHeartbeat;
    chatHeartbeatCount = 1;

    return false;
  }

  var adjustedHeight = chatboxtextarea.clientHeight;
  var maxHeight = 94;

  if (maxHeight > adjustedHeight) {
    adjustedHeight = Math.max(chatboxtextarea.scrollHeight, adjustedHeight);
    if (maxHeight) {
      adjustedHeight = Math.min(maxHeight, adjustedHeight);
    }
    if (adjustedHeight > chatboxtextarea.clientHeight) {
      jQuery(chatboxtextarea).css('height', adjustedHeight + 8 + 'px');
    }
  } else {
    jQuery(chatboxtextarea).css('overflow','auto');
  }
}
function setChatboxLogSetting(from_user, logStatus) {
  var logText = '';
  if (logStatus == Drupal.settings.googlechat.googlechatlog_status.off) {
    logStatus = Drupal.settings.googlechat.googlechatlog_status.on;
    logText = 'Go On the Records';
  }
  else if (logStatus == Drupal.settings.googlechat.googlechatlog_status.on) {
    logStatus = Drupal.settings.googlechat.googlechatlog_status.off;
    logText = 'Go Off the Records';
  }
  jQuery("#chatbox_" + from_user + " .chatbox_setting_more_list ul li:first").attr('value', logStatus);
  jQuery("#chatbox_" + from_user + " .chatbox_setting_more_list ul li:first").text(logText);
}

function startChatSession(){
  jQuery.getJSON(Drupal.settings.googlechat.startChatSessionUrl, function(data) {

  username = data.username;
  jQuery.each(data.items, function(i, item) {
    if (item != '{' && item != '}' && item.f != '')	{ /* fix strange ie bug*/

      chatboxtitle = formatChatBoxTitle(item.f);
      if (jQuery("#chatbox_" + chatboxtitle).length <= 0) {
        createChatBox(item.f, 1);
        if(data.chatlogUsers.hasOwnProperty(item.f)){
          var from_user = item.f;
          setChatboxLogSetting(from_user, data.chatlogUsers[from_user]);
        }
      }

      if (item.s == 1) {
        item.f = username;
      }
      item.m = linkify(item.m);
      msg = item.m.replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&quot;/g,"\"");

      if (item.s == 2) {
        jQuery("#chatbox_" + chatboxtitle + ">.chatboxcontent > .chatboxmessage:last-child>.chatboxusercontent").append('<span class="chatboxinfo sentat">' + msg + '</span>');
      } 
      else if (item.s == 1) {
        jQuery("#chatbox_" + chatboxtitle + " .chatboxcontent").append('<div class="chatboxmessage floatright"><div class="chatboxuserimage floatright"><img style="background: none repeat scroll 0px 0px rgb(0, 0, 0);" src="' + item.p + '" alt="' + item.r + '" title="' + item.r + '" width="30"/></div><div class="chatboxusercontent floatright"><span class="chatboxmessagecontent">' + msg + '</span></div><div class="msgtip tip_bottom"></div></div>');
      }
      else {
        jQuery("#chatbox_" + chatboxtitle + " .chatboxcontent").append('<div class="chatboxmessage"><div class="msgtip tip_top"></div><div class="chatboxuserimage"><img style="background: none repeat scroll 0px 0px rgb(0, 0, 0);" src="' + item.p + '" alt="' + item.r + '" title="' + item.r + '" width="30"/></div><div class="chatboxusercontent"><span class="chatboxmessagecontent">' + msg + '</span></div></div>');
      }
    }
  });

  for (i = 0; i < chatBoxes.length; i++) {
    chatboxtitle = chatBoxes[i];
    jQuery("#chatbox_" + chatboxtitle + " .chatboxcontent").scrollTop(jQuery("#chatbox_" + chatboxtitle + " .chatboxcontent")[0].scrollHeight);
    setTimeout('jQuery("#chatbox_"+chatboxtitle+" .chatboxcontent").scrollTop(jQuery("#chatbox_"+chatboxtitle+" .chatboxcontent")[0].scrollHeight);', 100); /* yet another strange ie bug*/
  }
  setTimeout('chatHeartbeat();', chatHeartbeatTime);

  });
  if (Drupal.settings.googlechat.userStatus == Drupal.settings.googlechat.googlechatStatus.away) {
    updateChatStatus(Drupal.settings.googlechat.googlechatStatus.online);
  }

}

jQuery.cookie = function(name, value, options) {
  if (typeof value != 'undefined') { /* name and value given, set cookie*/
    options = options || {};
    if (value === null) {
      value = '';
      options.expires = -1;
    }
    var expires = '';
    if (options.expires && (typeof options.expires == 'number' || options.expires.toUTCString)) {
      var date;
      if (typeof options.expires == 'number') {
        date = new Date();
        date.setTime(date.getTime() + (options.expires * 24 * 60 * 60 * 1000));
      } else {
        date = options.expires;
      }
      expires = '; expires=' + date.toUTCString(); /* use expires attribute, max-age is not supported by IE*/
    }
    // CAUTION: Needed to parenthesize options.path and options.domain
    // in the following expressions, otherwise they evaluate to undefined
    // in the packed version for some reason...
    var path = options.path ? '; path=' + (options.path) : '';
    var domain = options.domain ? '; domain=' + (options.domain) : '';
    var secure = options.secure ? '; secure' : '';
    document.cookie = [name, '=', encodeURIComponent(value), expires, path, domain, secure].join('');
  } else { /* only name given, get cookie*/
    var cookieValue = null;
    if (document.cookie && document.cookie != '') {
      var cookies = document.cookie.split(';');
      for (var i = 0; i < cookies.length; i++) {
        var cookie = jQuery.trim(cookies[i]);
        // Does this cookie string begin with the name we want?
        if (cookie.substring(0, name.length + 1) == (name + '=')) {
          cookieValue = decodeURIComponent(cookie.substring(name.length + 1));
          break;
        }
      }
    }
    return cookieValue;
  }
};

function gEmotions(smileSelecterIndex, message) {
  /*  smiley       code       title_text         alt_smilies           */
  var classicSmilies = {
          ":)"  : ["k7",     "happy",          ":-)"],
          ":D"  : ["j6",     "big grin",       ":-D,xD,XD"],
          ";)"  : ["lv",     "winking",        ";-)"],
          ":'(" : ["jj",     "crying",         ":'-("],
          ":-o" : ["kZ",     "surprise",       ":O,:-O,:o"],
          ":-/" : ["k3",     "confused",       ":/"],
          "x-(" : ["i0",     "angry",          "X(,x(,X-("],
          ":("  : ["j2",     "crying",         ":-((,:(,:(("],
          "B-)" : ["jf",     "cool",           ""],
          ":P"  : ["lr",     "tongue",         ":p,:-p,:-P"],
          "<3"  : ["ka",     "heart",          ""],
          ":-|" : ["lb",     "straight face",   ":|"]
      };
  var roundSmilies = {
          ":)"  : ["k9",     "happy",          ":-)"],
          ":D"  : ["j8",     "big grin",       ":-D,xD,XD"],
          ";)"  : ["lx",     "winking",        ";-)"],
          ":'(" : ["jl",     "crying",         ":'-("],
          ":-o" : ["k1",     "surprise",       ":O,:-O,:o"],
          ":-/" : ["k5",     "confused",       ":/"],
          "x-(" : ["i2",     "angry",          "X(,x(,X-("],
          ":("  : ["j4",     "crying",         ":-((,:(,:(("],
          "B-)" : ["jh",     "cool",           ""],
          ":P"  : ["lt",     "tongue",         ":p,:-p,:-P"],
          "<3"  : ["kc",     "heart",          ""],
          ":-|" : ["ld",     "straight face",   ":|"]
      };
  var squareSmilies = {
          ":)"  : ["k8",     "happy",          ":-)"],
          ":D"  : ["j7",     "big grin",       ":-D,xD,XD"],
          ";)"  : ["lw",     "winking",        ";-)"],
          ":'(" : ["jk",     "crying",         ":'-("],
          ":-o" : ["k0",     "surprise",       ":O,:-O,:o"],
          ":-/" : ["k4",     "confused",       ":/"],
          "x-(" : ["i1",     "angry",          "X(,x(,X-("],
          ":("  : ["j3",     "crying",         ":-((,:(,:(("],
          "B-)" : ["jg",     "cool",           ""],
          ":P"  : ["ls",     "tongue",         ":p,:-p,:-P"],
          "<3"  : ["kb",     "heart",          ""],
          ":-|" : ["lc",     "straight face",   ":|"]
     };
  var mixColorSmilies = {
          ":)"  : ["k6",     "happy",          ":-)"],
          ":D"  : ["j5",     "big grin",       ":-D,xD,XD"],
          ";)"  : ["lu",     "winking",        ";-)"],
          ":'(" : ["ji",     "crying",         ":'-("],
          ":-o" : ["kY",     "surprise",       ":O,:-O,:o"],
          ":-/" : ["k2",     "confused",       ":/"],
          "x-(" : ["iZ",     "angry",          "X(,x(,X-("],
          ":("  : ["j1",     "crying",         ":-((,:(,:(("],
          "B-)" : ["je",     "cool",           ""],
          ":P"  : ["lq",     "tongue",         ":p,:-p,:-P"],
          "<3"  : ["j9",     "heart",          ""],
          ":-|" : ["la",     "straight face",   ":|"]
     };
  var smiles = {0 :classicSmilies, 1 : roundSmilies, 2 : squareSmilies, 3 : mixColorSmilies};
  var selectedSmileSet;
  selectedSmileSet = smiles[smileSelecterIndex];
  message = escape(message);

  jQuery.each(selectedSmileSet, function(keySmile, smileBox) {
    smileBoxArr = [];
    if(smileBox[2] != "") {
      smileBoxArr = smileBox[2].split(',');
    }
    smileBoxArr.push(keySmile);
    jQuery.each(smileBoxArr, function(key, val) {
      tag = '<img style="left:0px;" alt="' + smileBox[1] + '" title="' + smileBox[1] + '" src="' + Drupal.settings.googlechat.emoticonsURL + '" class="' + smileBox[0] + '"></img>';
      message = message.replace(new RegExp(escape(val), "g"), tag);
    });
  });

  message = unescape(message);
  return message;
}

function linkify(inputText) {

  var linkClass, targetBlank;
  linkClass = 'url';
  targetBlank = true;

  inputText = inputText.replace(/\u200B/g, "");

  //URLs starting with http://, https://, or ftp:// like http://example.com or http://www.example.com
  var replacePattern1 = /(src="|" | href="|">|\s>)?(https?|ftp):\/\/[-A-Z0-9+&@#\/%?=~_|!:,.;ï]*[-A-Z0-9+&@#\/%=~_|ï]/gim;
  var replacePattern1 = /(src="|href="|">|\s>)?(https?|ftp):\/\/[-A-Z0-9+&@#\/%?=~_|!:,.;ï]*[-A-Z0-9+&@#\/%=~_|ï]/gim;
  var replacedText = inputText.replace(replacePattern1, function($0,$1){ return $1?$0:'<a class="' + linkClass + '" href="' + $0 + '"' + (targetBlank?'target="_blank"':'') + '>' + $0 + '</a>';});

  //URLS starting with www and not the above like www.example.com
  var replacePattern2 = /(src="|href="|">|\s>|https?:\/\/|ftp:\/\/)?www\.[-A-Z0-9+&@#\/%?=~_|!:,.;ï]*[-A-Z0-9+&@#\/%=~_|ï]/gim;
  var replacedText = replacedText.replace(replacePattern2, function($0,$1){ return $1?$0:'<a class="' + linkClass + '" href="http://' + $0 + '"' + (targetBlank?'target="_blank"':'') + '>' + $0 + '</a>';});

  //Change email addresses to mailto:: links
  var replacePattern3 = /([\.\w]+@[a-zA-Z_]+?\.[a-zA-Z]{2,6})/gim;
  var replacedText = replacedText.replace(replacePattern3, '<a class="' + linkClass + '" href="mailto:$1">$1</a>');

  return replacedText;
}
