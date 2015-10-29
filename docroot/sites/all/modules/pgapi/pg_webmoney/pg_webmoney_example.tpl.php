<h1>Merchant purse settings</h1>

<table>
  <tbody>

    <tr>
      <td nowrap="nowrap">Purse:</td>
      <td class="header3" align="left"><b><?php print $purse; ?></b></td>
      <td>&nbsp;</td>
      <td align="left">&nbsp;</td>
    </tr>

    <tr>
      <td nowrap="nowrap">Trade Name:</td>
      <td align="left"><input style="display: inline;" value="<?php print $tradename; ?>" size="50" id="m_name" name="m_name" type="text"></td>
      <td align="center">&nbsp;-&nbsp;</td>
      <td align="left"> is displayed on the web page during the payment </td>
    </tr>

    <tr>
      <td nowrap="nowrap">Secret Key:</td>
      <td align="left"><input style="display: inline;" value="<?php print $secret_key; ?>" id="secret_key" name="secret_key" size="50" type="text"></td>
      <td align="center">
        <input checked="checked" id="send_secret_key" name="send_secret_key" type="checkbox"></td>
      <td align="left">Send the Secret Key to the Result URL if the Result URL is secured</td>
    </tr>

    <tr>
      <td nowrap="nowrap">Result URL:</td>
      <td align="left"><input style="display: inline;" value="<?php print $done; ?>" id="result_url" name="result_url" size="50" maxlength="255" type="text"></td>
      <td align="center">
        <input checked="checked" id="send_param_prerequest" name="send_param_prerequest" value="1" type="checkbox">
      </td>
      <td align="left">Send parameters in the pre-request</td>
    </tr>

    <tr>
      <td nowrap="nowrap">Success URL:</td>
      <td align="left"><input style="display: inline;" value="<?php print $success; ?>" id="success_url" name="success_url" size="50" maxlength="255" type="text"></td>
      <td align="center">
        <select name="success_method" id="success_method">
          <option value="0">GET</option>
          <option value="1" selected="selected" >POST</option>
          <option value="2">LINK</option>
        </select>
      </td>
      <td>method of requesting Success URL</td>
    </tr>

    <tr>
      <td nowrap="nowrap">Fail URL:</td>
      <td align="left"><input style="display: inline;" value="<?php print $fail; ?>" id="fail_url" name="fail_url" size="50" maxlength="255" type="text"></td>
      <td align="center">
        <select name="fail_method" id="fail_method">
          <option value="0">GET</option>
          <option value="1" selected="selected">POST</option>
          <option value="2">LINK</option>
        </select>
      </td>
      <td>method of requesting Fail URL</td>
    </tr>

  </tbody>
</table>

<table>
  <tbody>

    <tr>
      <td nowrap="nowrap">Allow overriding URL from Payment Request Form:</td>
      <td align="left">
        <input id="allow_url_from_form" name="allow_url_from_form" value="1" type="checkbox">
      </td>
      <td></td>
      <td></td>
    </tr>

    <tr>
      <td nowrap="nowrap">Send an error notification to merchant\'s keeper:</td>
      <td align="left">
        <input checked="checked" id="send_error" name="send_error" value="1" type="checkbox">
      </td>
      <td></td>
      <td></td>
    </tr>

    <tr>
      <td nowrap="nowrap">Signature algorithm:</td>
      <td align="left">
        <select name="auth_type" id="auth_type">
          <option value="0">SIGN</option>
          <option value="1" selected="selected">MD5</option>
        </select>
      </td>
      <td></td>
      <td></td>
    </tr>

  </tbody>
</table>
