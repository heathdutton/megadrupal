<?php print '<?xml version="1.0" encoding="' . $encoding . '"?>' ?>
<xfdf xmlns="http://ns.adobe.com/xfdf/" xml:space="preserve">
  <f href="<?php print $filename ?>"/>
  <ids original="<?php print $stamp ?>" modified="<?php print $stamp ?>"/>
  <fields>
    <?php foreach ($info as $field => &$val): ?>
      <field name="<?php print $field ?>">
        <?php if (is_array($val)): ?>
          <?php foreach ($val as &$opt): ?>
            <value><?php print htmlentities($opt, ENT_COMPAT, $encoding) ?></value>
          <?php endforeach ?>
        <?php else: ?>
          <value><?php print htmlentities($val, ENT_COMPAT, $encoding) ?></value>
        <?php endif ?>
      </field>
    <?php endforeach ?>
  </fields>
</xfdf>