/**
 * Put here the pfcClient, pfcGui, pfcResources customizations
 * ex: you can override the pfcClient::updateNickList methode
 *     in order to display links on the nicknames (see demo34 for a concrete example)
 */

  /**
   * React to the server response
   */
  pfcClient.prototype.handleResponse = function(cmd, resp, param)
  {
    // display some debug messages
    if (pfc_debug)
      if (cmd != "update")
      {
        var param2 = param;
        if (cmd == "who" || cmd == "who2")
        {
          param2 = $H(param2);
          param2.set('meta', $H(param2.get('meta')));
          param2.get('meta').set('users', $H(param2.get('meta').get('users')));
          trace('handleResponse: '+cmd + "-"+resp+"-"+param2.inspect());
        }
        else
        if (cmd == "whois" || cmd == "whois2")
        {
          param2 = $H(param2);
          trace('handleResponse: '+cmd + "-"+resp+"-"+param2.inspect());
        }
        else
        if (cmd == "getnewmsg" || cmd == "join")
        {
          param2 = $A(param2);
          trace('handleResponse: '+cmd + "-"+resp+"-"+param2.inspect());
        }
        else
          trace('handleResponse: '+cmd + "-"+resp+"-"+param);
      }

    if (cmd != "update")
    {
       // speed up timeout if activity
       this.last_activity_time = new Date().getTime();
       var delay = this.calcDelay();
       if (this.timeout_time - new Date().getTime() > delay)
       {
          clearTimeout(this.timeout);
          this.timeout = setTimeout('pfc.updateChat(true)', delay);
          this.timeout_time = new Date().getTime() + delay;
       }
    }

    if (cmd == "connect")
    {
      if (resp == "ok")
      {
        this.nickname = param[0];
        this.isconnected = true;

        // start the polling system
        this.updateChat(true);
      }
      else
        this.isconnected = false;
      this.refresh_loginlogout();
    }
    else if (cmd == "quit")
    {
      if (resp =="ok")
      {
        // stop updates
        this.updateChat(false);
        this.isconnected = false;
        this.refresh_loginlogout();
      }
    }
    else if (cmd == "join" || cmd == "join2")
    {
      if (resp =="ok")
      {
        // create the new channel
        var tabid = param[0];
        var name  = param[1];
        this.gui.createTab(name, tabid, "ch");
        if (cmd != "join2" || this.gui.tabs.length == 1) this.gui.setTabById(tabid);
        this.refresh_Smileys();
        this.refresh_WhosOnline();
      }
      else if (resp == "max_channels")
      {
        this.displayMsg( cmd, this.res.getLabel('Maximum number of joined channels has been reached') );
      }
      else
        alert(cmd + "-"+resp+"-"+param);
    }
    else if (cmd == "leave")
    {
      if (resp =="ok")
      {
        // remove the channel
        var tabid = param;
        this.gui.removeTabById(tabid);

        // synchronize the channel client arrays
        /*
        var index = -1;
        index = this.channelids.indexOf(tabid);
        this.channelids = this.channelids.without(tabid);
        this.channels   = this.channels.without(this.channels[index]);
        */

        // synchronize the privmsg client arrays
        index = -1;
        index = indexOf(this.privmsgids, tabid);
        this.privmsgids = without(this.privmsgids, tabid);
        this.privmsgs   = without(this.privmsgs, this.privmsgs[index]);

      }
    }
    else if (cmd == "privmsg" || cmd == "privmsg2")
    {
      if (resp == "ok")
      {
        // create the new channel
        var tabid = param[0];
        var name  = param[1];
        this.gui.createTab(name, tabid, "pv");
        if (cmd != "privmsg2" || this.gui.tabs.length == 1) this.gui.setTabById(tabid);

        this.privmsgs.push(name);
        this.privmsgids.push(tabid);

      }
      else if (resp == "max_privmsg")
      {
        this.displayMsg( cmd, this.res.getLabel('Maximum number of private chat has been reached') );
      }
      else if (resp == "unknown")
      {
        // speak to unknown user
        this.displayMsg( cmd, this.res.getLabel('You are trying to speak to a unknown (or not connected) user') );
      }
      else if (resp == "speak_to_myself")
      {
        this.displayMsg( cmd, this.res.getLabel('You are not allowed to speak to yourself') );
      }
      else
        alert(cmd + "-"+resp+"-"+param);
    }
    else if (cmd == "nick")
    {
      // give focus the the input text box if wanted
      if (pfc_focus_on_connect) this.el_words.focus();

      if (resp == "connected" || resp == "notchanged")
      {
        cmd = '';
      }

      if (resp == "ok" || resp == "notchanged" || resp == "changed" || resp == "connected")
      {
        this.setUserMeta(this.nickid, 'nick', param);
        this.el_handle.innerHTML = this.getUserMeta(this.nickid, 'nick').escapeHTML();
        this.nickname = this.getUserMeta(this.nickid, 'nick');
        this.updateNickBox(this.nickid);

        // clear the possible error box generated by the bellow displayMsg(...) function
        this.clearError(Array(this.el_words));
      }
      else if (resp == "isused")
      {
        this.setError(this.res.getLabel('Chosen nickname is already used'), Array());
        this.askNick(param,this.res.getLabel('Chosen nickname is already used'));
      }
      else if (resp == "notallowed")
      {
        // When frozen_nick is true and the nickname is already used, server will return
        // the 'notallowed' status. It will display a message and stop chat update.
        // If the chat update is not stopped, this will loop forever
        // as long as the forced nickname is not changed.

        // display a message
        this.setError(this.res.getLabel('Chosen nickname is not allowed'), Array());
        // then stop chat updates
        this.updateChat(false);
        this.isconnected = false;
        this.refresh_loginlogout();
      }
    }
    else if (cmd == "update")
    {
    }
    else if (cmd == "version")
    {
      if (resp == "ok")
      {
        this.displayMsg( cmd, this.res.getLabel('phpfreechat current version is %s',param) );
      }
    }
    else if (cmd == "help")
    {
      if (resp == "ok")
      {
        this.displayMsg( cmd, param);
      }
    }
    else if (cmd == "rehash")
    {
      if (resp == "ok")
      {
        this.displayMsg( cmd, this.res.getLabel('Configuration has been rehashed') );
      }
      else if (resp == "ko")
      {
        this.displayMsg( cmd, this.res.getLabel('A problem occurs during rehash') );
      }
    }
    else if (cmd == "banlist")
    {
      if (resp == "ok" || resp == "ko")
      {
        this.displayMsg( cmd, param );
      }
    }
    else if (cmd == "unban")
    {
      if (resp == "ok" || resp == "ko")
      {
        this.displayMsg( cmd, param );
      }
    }
    else if (cmd == "auth")
    {
      if (resp == "ban")
      {
        alert(param);
      }
      if (resp == "frozen")
      {
        alert(param);
      }
      else if (resp == "nick")
      {
        this.displayMsg( cmd, param );
      }
    }
    else if (cmd == "debug")
    {
      if (resp == "ok" || resp == "ko")
      {
        this.displayMsg( cmd, param );
      }
    }
    else if (cmd == "clear")
    {
      var tabid     = this.gui.getTabId();
      var container = this.gui.getChatContentFromTabId(tabid);
      container.innerHTML = "";
    }
    else if (cmd == "identify")
    {
      this.displayMsg( cmd, param );
    }
    else if (cmd == "checknickchange")
    {
      this.displayMsg( cmd, param );
    }
    else if (cmd == "whois" || cmd == "whois2")
    {
      param = $H(param);
      var nickid = param.get('nickid');
      if (resp == "ok")
      {
        this.setUserMeta(nickid, param);
        this.updateNickBox(nickid);
        this.updateNickWhoisBox(nickid);
      }
      if (cmd == "whois")
      {
        // display the whois info
        var um = this.getAllUserMeta(nickid);
        var um_keys = um.keys();
        var msg = '';
        for (var i=0; i<um_keys.length; i++)
        {
          var k = um_keys[i];
          var v = um.get(k);
          if (v &&
              // these parameter are used internaly (don't display it)
              k != 'nickid' &&
              k != 'floodtime' &&
              k != 'flood_nbmsg' &&
              k != 'flood_nbchar')
            msg = msg + '<strong>' + k + '</strong>: ' + v + '<br/>';
        }
        this.displayMsg( cmd, msg );
      }
    }
    else if (cmd == "who" || cmd == "who2")
    {
      param = $H(param);
      var chan   = param.get('chan');
      var chanid = param.get('chanid');
      var meta   = $H(param.get('meta'));
      meta.set('users', $H(meta.get('users')));
      if (resp == "ok")
      {
        this.setChanMeta(chanid,meta);
        // send /whois commands for unknown users
        for (var i=0; i<meta.get('users').get('nickid').length; i++)
        {
          var nickid = meta.get('users').get('nickid')[i];
          var nick   = meta.get('users').get('nick')[i];
          var um = this.getAllUserMeta(nickid);
          if (!um) this.sendRequest('/whois2 "'+nickid+'"');
        }

        // update the nick list display on the current channel
        this.updateNickListBox(chanid);
      }
      if (cmd == "who")
      {
        // display the whois info
        var cm = this.getAllChanMeta(chanid);
        var cm_keys = cm.keys();
        var msg = '';
        for (var i=0; i<cm_keys.length; i++)
        {
          var k = cm_keys[i];
          var v = cm[k];
          if (k != 'users')
          {
            msg = msg + '<strong>' + k + '</strong>: ' + v + '<br/>';
          }
        }
        this.displayMsg( cmd, msg );
      }
    }
    else if (cmd == "getnewmsg")
    {
      if (resp == "ok")
      {
        this.handleComingRequest(param);
      }
    }
    else if (cmd == "send")
    {
    }
	else if (cmd == "away")
	{
		var awayBtn=document.getElementById("pfc_onOffAwayBtn");	
		if (awayBtn)
		{
			if (resp == 'yes')
			{
			  awayBtn.src   = param;
			  awayBtn.alt   = 'Set away status off';
			  awayBtn.title = awayBtn.alt;
			}
			else
			{
			  awayBtn.src   = param;
			  awayBtn.alt   = 'Set away status on';
			  awayBtn.title = awayBtn.alt;
			}
		}
	}
    else
      alert(cmd + "-"+resp+"-"+param);
  }


  pfcClient.prototype.updateNickWhoisBox = function(nickid)
  {
    var className = (! is_ie) ? 'class' : 'className';

    var usermeta = this.getAllUserMeta(nickid);
    var div  = document.createElement('div');
    div.setAttribute(className, 'pfc_nickwhois');

    var p = document.createElement('p');
    p.setAttribute(className, 'pfc_nickwhois_header');
    div.appendChild(p);

    // add the close button
    var img = document.createElement('img');
    img.setAttribute(className, 'pfc_nickwhois_close');
    img.pfc_parent = div;
    img.onclick = function(evt){
      this.pfc_parent.style.display = 'none';
      return false;
    }
    img.setAttribute('src', this.res.getFileUrl('images/close-whoisbox.gif'));
    img.alt = this.res.getLabel('Close');
    p.appendChild(img);
    p.appendChild(document.createTextNode(usermeta.get('nick'))); // append the nickname text in the title

    // add the whois information table
    var table = document.createElement('table');
    var tbody = document.createElement('tbody');
    table.appendChild(tbody);
    var um_keys = usermeta.keys();
    var msg = '';
    for (var i=0; i<um_keys.length; i++)
    {
      var k = um_keys[i];
      var v = usermeta.get(k);
      if (v && k != 'nickid'
            && k != 'nick' // useless because it is displayed in the box title
            && k != 'isadmin' // useless because of the gold shield icon
            && k != 'floodtime'
            && k != 'flood_nbmsg'
            && k != 'flood_nbchar'
			&& k != 'drupal_base_url'
			&& k != 'drupal_user_uid'
         )
      {
        var tr = document.createElement('tr');
        if (pfc_nickmeta_key_to_hide.indexOf(k) != -1)
        {
          var td2 = document.createElement('td');
          td2.setAttribute(className, 'pfc_nickwhois_c2');
          td2.setAttribute('colspan', 2);
          td2.innerHTML = v;
          tr.appendChild(td2);
        }
        else
        {
          var td1 = document.createElement('td');
          td1.setAttribute(className, 'pfc_nickwhois_c1');
          var td2 = document.createElement('td');
          td2.setAttribute(className, 'pfc_nickwhois_c2');
          td1.innerHTML = k;
          td2.innerHTML = v;
          tr.appendChild(td1);
          tr.appendChild(td2);
        }
        tbody.appendChild(tr);
      }
    }
    div.appendChild(table);
	
	// create a link to the drupal profile.
    var p = document.createElement('p');
    p.setAttribute(className, 'pfc_nickwhois_pv');
    var a = document.createElement('a');
	var drupal_profile_url = pfc.getUserMeta(nickid,'drupal_base_url')+'/user/'+pfc.getUserMeta(nickid,'drupal_user_uid');
    a.setAttribute('href', drupal_profile_url);
    a.setAttribute('target', '_blank');
    img = document.createElement('img');
    img.setAttribute('src', this.res.getFileUrl('images/user.gif'));
    img.alt = document.createTextNode('Drupal Profile');
    a.appendChild(img);
    a.pfc_parent = div;
    a.onclick = function(evt){
	  window.open(drupal_profile_url);
      this.pfc_parent.style.display = 'none';
      return false;
    }
    a.appendChild(document.createTextNode('Drupal Profile'));
    p.appendChild(a);
    div.appendChild(p);
		
    // add these links only if the nick is not yours
    if (pfc.getUserMeta(nickid,'nick') != this.nickname)
    {
      // show kick/ban in whois box:
      // first make sure the current user is admin
      var pfc_isadmin = this.getUserMeta(pfc_nickid, 'isadmin');
      // this might be useful if you do not want kick/ban appear
      // in whois box of an admin
      var isadmin = this.getUserMeta(nickid, 'isadmin');
      // only show kick/ban for admin users
      if (pfc_isadmin) {
        // ban
        var p = document.createElement('p');
        p.setAttribute(className, 'pfc_nickwhois_pv');
        a = document.createElement('a');
        a.setAttribute('href', '');
        a.pfc_nickid = nickid;
        a.pfc_parent = div;
        a.onclick = function(evt) {
          var nick = pfc.getUserMeta(this.pfc_nickid,'nick');
          pfc.sendRequest('/ban "'+nick+'"');
          this.pfc_parent.style.display = 'none';
          return false;
        }
        img = document.createElement('img');
        img.setAttribute('src', this.res.getFileUrl('images/logout.gif'));
        img.alt = document.createTextNode('Ban user');
        a.appendChild(img);
        a.appendChild(document.createTextNode('Ban user'));
        p.appendChild(a);
        div.appendChild(p);

        // kick
        var p = document.createElement('p');
        p.setAttribute(className, 'pfc_nickwhois_pv');
        var a = document.createElement('a');
        a.setAttribute('href', '');
        a.pfc_nickid = nickid;
        a.pfc_parent = div;
        a.onclick = function(evt){
          var nick = pfc.getUserMeta(this.pfc_nickid,'nick');
          pfc.sendRequest('/kick "'+nick+'"');
          this.pfc_parent.style.display = 'none';
          return false;
        }
        var img = document.createElement('img');
        img.setAttribute('src', this.res.getFileUrl('images/logout.gif'));
        img.alt = document.createTextNode('Kick user');
        a.appendChild(img);
        a.appendChild(document.createTextNode('Kick user'));
        p.appendChild(a);
        div.appendChild(p);
      }

      // add the privmsg link only if the user is not away
      if (pfc.getUserMeta(nickid,'away') == 'no')
      {
        var p = document.createElement('p');
        p.setAttribute(className, 'pfc_nickwhois_pv');
        var a = document.createElement('a');
        a.setAttribute('href', '');
        a.pfc_nickid = nickid;
        a.pfc_parent = div;
        a.onclick = function(evt){
          var nick = pfc.getUserMeta(this.pfc_nickid,'nick');
          pfc.sendRequest('/privmsg "'+nick+'"');
          this.pfc_parent.style.display = 'none';
          return false;
        }
        var img = document.createElement('img');
        img.setAttribute('src', this.res.getFileUrl('images/openpv.gif'));
        img.alt = document.createTextNode(this.res.getLabel('Private message'));
        a.appendChild(img);
        a.appendChild(document.createTextNode(this.res.getLabel('Private message')));
        p.appendChild(a);
        div.appendChild(p);
      }
    }
    this.nickwhoisbox.set(nickid, div);
  }


  pfcClient.prototype.buildNickItem = function(nickid)
  {
    var className = (! is_ie) ? 'class' : 'className';

    var nick = this.getUserMeta(nickid, 'nick');
    var isadmin = this.getUserMeta(nickid, 'isadmin');
    if (isadmin == '') isadmin = false;

    var li = document.createElement('li');

    var a = document.createElement('a');
    a.setAttribute('href','#');
    a.pfc_nick   = nick;
    a.pfc_nickid = nickid;
    a.onclick = function(evt){
      var d = pfc.getNickWhoisBox(this.pfc_nickid);
      document.body.appendChild(d);
      d.style.display = 'block';
      d.style.zIndex = '400';
      d.style.position = 'absolute';
      d.style.left = (mousePosX(evt)-7)+'px';
      d.style.top  = (mousePosY(evt)-7)+'px';
      return false;
    }
    li.appendChild(a);

    var img = document.createElement('img');
    if (isadmin)
      img.setAttribute('src', this.res.getFileUrl('images/user-admin.gif'));
    else
      img.setAttribute('src', this.res.getFileUrl('images/user.gif'));
    img.style.marginRight = '5px';
    img.setAttribute(className, 'pfc_nickbutton');
    a.appendChild(img);

    // nobr is not xhtml valid but it's a workaround
    // for IE which doesn't support 'white-space: pre' css rule
    var nobr = document.createElement('nobr');
    var span = document.createElement('span');
    span.setAttribute(className, 'pfc_nickmarker pfc_nick_'+nickid);
/*
	// Problems with IE and innerHTML
    span.innerHTML = nick.escapeHTML();
	if (this.getUserMeta(nickid, 'away') == 'yes')
		span.innerHTML += ' (away)';
*/
	var nicktext = document.createTextNode(nick.escapeHTML());
    if (this.getUserMeta(nickid, 'away') == 'yes') {
		nicktext.data += ' (away)';
	}

span.appendChild(nicktext);

    nobr.appendChild(span);
    a.appendChild(nobr);

    return li;
  }



