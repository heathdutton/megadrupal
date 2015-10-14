<?php
/**
 * This is just a page thats allows users can type tamil.
 *
 * @return
 *  The page of one text area and help info
 */
function indic_script_tamil_type_page_demo() {
  $ourput = drupal_get_form('indic_script_tamil_type_page');
  return $ourput;
}

/**
 * This is just a page thats allows users can type tamil.
 *
 * @return
 *  The page of one text area and help info
 */
function indic_script_tamil_type_page(){
  $form = array();
  $form['body'] = array(
    '#type' => 'textarea',
    '#title' => t('Type here'),
    '#description' => t(''),
    '#rows' => 7,
    '#weight' => 0,
    '#attributes' => array(
      'class' => array('kanni-enabled'),
    ),
  );
  $form['is_help'] = array(
    '#type' => 'item',
    '#markup' => _indic_script_tamil_page_help(),
    '#weight' => 1,
  );
  return $form;
}

/**
 * This is just a page thats allows users can type tamil.
 *
 * @return
 *  The page of one text area and help info
 */
function _indic_script_tamil_page_help(){

  $output = '
<div><strong>யுனித்தமிழ் எழுத்துக்கள் அட்டவணை</strong> <em>(ஆங்கிலத்தில் எழுத F9 தேர்வு செய்யவும்)</em><br />
<table>
    <tbody>
        <tr>
            <td><strong>உயிரெழுத்துக்கள்</strong></td>
            <td><strong>மெய்யெழுத்துக்கள்</strong></td>
            <td><strong>கிரந்த எழுத்துக்கள்</strong></td>
        </tr>
        <tr>
            <td valign="top">அ - a<br />
            ஆ - aa, A<br />
            இ - i<br />
            ஈ - ii, I<br />
            உ - u<br />
            ஊ - uu, U<br />
            எ - e<br />
            ஏ - ee, E<br />
            ஐ - ai<br />
            ஒ - o<br />
            ஓ - oo, O<br />
            ஔ - au<br />
  <strong>ஆய்த எழுத்து</strong><br />
              ஃ - q<br />
            &nbsp;</td>
            <td valign="top">க் - k, g<br />
            ங் - ng<br />
            ச் - c, s<br />
            ஞ் - nj, X<br />
            ட் - t, d<br />
            ண் - N<br />
            த் - th, dh, T<br />
            ந் - w, n-<br />
            ப் - p, b<br />
            ம் - m<br />
            ய் - y<br />
            ர் - r<br />
            ல் - l<br />
            வ் - v<br />
            ழ் - z<br />
            ள் - L<br />
            ற் - R<br />
            ன் - n<br />
            &nbsp;</td>
            <td valign="top">ஜ் - j<br />
            ஷ் - sh, ch, Z<br />
            ஸ் - S<br />
            ஹ் - h<br />
            க்ஷ் - ksh, kch, kZ<br />
            ஸ்ரீ - Srii</td>
        </tr>
    </tbody>
</table>
<div class="messages">
This is the demo page of <a target="_blank" href="http://drupal.org/project/indic_script">Indic Script</a> module built for <a target="_blank" href="http://drupal.org">Drupal</a>. <br />
see http://drupal.org/project/indic_script for more details.
</div>
</div>';

  return $output;
}
