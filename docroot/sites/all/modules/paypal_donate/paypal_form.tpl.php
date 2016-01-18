<?php ?>
<div class="node_<?php print $node->nid ?>">
    <?php print $node->fields['body']; ?>

    <form action="https://www.paypal.com/cgi-bin/webscr" method="post">
        <input type="hidden" name="cmd" value="_donations">
        <input type="hidden" name="business" value="<?php print $node->fields['paypal_donate_email']; ?>">
        <input type="hidden" name="item_name" value="Donation">
        <input type="hidden" name="currency_code" value="<?php print $node->fields['paypal_donate_currency']; ?>">
        <input type="hidden" name="bn" value="PP-DonationsBF:btn_donateCC_LG.gif:NonHostedGuest">
        <input type="image" src="https://www.paypal.com/en_US/i/btn/btn_donateCC_LG.gif" border="0" name="submit" alt="PayPal - The safer, easier way to pay online!">
        <img alt="" border="0" src="https://www.paypal.com/en_US/i/scr/pixel.gif" width="1" height="1">
    </form>
</div>
