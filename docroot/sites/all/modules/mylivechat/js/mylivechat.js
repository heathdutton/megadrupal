function MyLiveChat_Trim(str) {
    if (typeof(str)=="undefined" || typeof(str) !="string" || !str)
        return null;
    return str.replace(/(^\s*)/g, "").replace(/(\s*$)/g, "");
}

function MyLiveChat_IsValidEmail(str) {
    var reg = /^([\w-\.]+)@((\[[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.)|(([\w-]+\.)+))([a-zA-Z]{2,4}|[0-9]{1,3})(\]?)$/;
    return reg.test(str);
}

function MyLiveChat_UnsafeChar(str)
{
	if(str.indexOf("<")!=-1 || str.indexOf(">")!=-1 || 
		str.indexOf("&")!=-1 || str.indexOf("'")!=-1 || str.indexOf("\"")!=-1)
	{
		return true;
	}
	return false;
}

function MyLiveChat_GMT()
{
	var date, dateGMTString, date2, gmt;

	date = new Date((new Date()).getFullYear(), 0, 1, 0, 0, 0, 0);
	dateGMTString = date.toGMTString();
	date2 = new Date(dateGMTString.substring(0, dateGMTString.lastIndexOf(" ")-1));
	gmt = ((date - date2) / (1000 * 60 * 60)).toString();

	return gmt;
}

function MyLiveChat_Show_Signup()
{
	var c1 = document.getElementById("Cont_MyLiveChat_Setting");
	var c2 = document.getElementById("Cont_MyLiveChat_Signup");
	var btn = document.getElementById("edit-submit");
	if(c1)
		c1.style.display = "none";
	if(c2)
		c2.style.display = "";
	if(btn)
	{
		btn.setAttribute("type","button");
		btn.onclick = MyLiveChat_Signup;
		btn.value = "Create account";
	}
}

function MyLiveChat_Show_Setting()
{
	var c1 = document.getElementById("Cont_MyLiveChat_Setting");
	var c2 = document.getElementById("Cont_MyLiveChat_Signup");
	var btn = document.getElementById("edit-submit");
	if(c1)
		c1.style.display = "";
	if(c2)
		c2.style.display = "none";
	if(btn)
	{
		btn.setAttribute("type","submit");
		btn.onclick = function(){};
		btn.value = "Save configuration";
	}
}

function MyLiveChat_Signup()
{
	var email = MyLiveChat_Trim(document.getElementById("edit-email").value);
	var password = MyLiveChat_Trim(document.getElementById("edit-password").value);
	var password_retype = MyLiveChat_Trim(document.getElementById("edit-password-retype").value);
	var firstname = MyLiveChat_Trim(document.getElementById("edit-firstname").value);
	var lastname = MyLiveChat_Trim(document.getElementById("edit-lastname").value);
	if(!email)
	{
		alert("Email is required");
		return;
	}
	if(MyLiveChat_UnsafeChar(email))
	{
		alert("Email should not contain special characters");
		return;
	}
	if(!MyLiveChat_IsValidEmail(email))
	{
		alert("Incorrect email format");
		return;
	}
	if(!password)
	{
		alert("Password is required");
		return;
	}
	if(password.length<=2)
	{
		alert("Password is too short");
		return;
	}
	if(password!==password_retype)
	{
		alert("Retype password is not equal with password");
		return;
	}
	if(!firstname)
	{
		alert("First name is required");
		return;
	}
	if(MyLiveChat_UnsafeChar(firstname))
	{
		alert("First name should not contain special characters");
		return;
	}
	if(!lastname)
	{
		alert("Last name is required");
		return;
	}
	if(MyLiveChat_UnsafeChar(lastname))
	{
		alert("Last name should not contain special characters");
		return;
	}
	
	var url;

	$('#Cont_MyLiveChat_Signup .ajax_message').removeClass('message').addClass('wait').html('Creating new account&hellip;');

	url = 'https://www.mylivechat.com/addon-signup.aspx';
	url += '?firstname='+encodeURIComponent(firstname);
	url += '&lastname='+encodeURIComponent(lastname);
	url += '&email='+encodeURIComponent(email);
	url += '&password='+encodeURIComponent(password);
	url += '&timezone_gmt='+encodeURIComponent(MyLiveChat_GMT());
	url += '&action=drupal_signup';
	url += '&jsoncallback=?';

	$.getJSON(url, function(data)
	{
		data = parseInt(data.r);
		if (data == 0)
		{
			$('#Cont_MyLiveChat_Signup .ajax_message').html('<span style="color:red;">Could not create account. Please try again later.</span>').addClass('message').removeClass('wait');
			return false;
		}

		if(data == 1)
		{
			$('#Cont_MyLiveChat_Signup .ajax_message').html('<span style="color:red;">An account has already been created for your entered Email Address!</span>').addClass('message').removeClass('wait');
			return false;
		}

		// save mylivechat id
		$('#edit-mylivechat-id').val(data);
		$('#mylivechat-admin-settings-form').submit();
	});
}