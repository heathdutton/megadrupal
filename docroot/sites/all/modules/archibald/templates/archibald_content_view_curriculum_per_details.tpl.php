<?php
/**
 * @file
 * archibald_content_view_curriculum_educa.tpl.php
 * display details model for curriculum per
 *
 * Parent file:
 *  archibald_content_view_curriculum_per.tpl.php
 *
 * Available variables:
 * - $objectiv: a singel objectiv from a curriculum per entry
 * - $possible_school_year: list of posible school year`s
 * - $css_base_code
 * - $action: The current action
 *    view: display the lom resource
 *    edit: edit the lom resource
 */
?>
<h2 class="<?PHP echo $css_base_code; ?>"><?PHP echo $objectiv->code; ?> — <?PHP echo $objectiv->description; ?></h2>

<table class="t_progression">
<thead>
    <tr class="headers1">
        <th colspan="<?PHP echo count($possible_school_year); ?>">Progression des apprentissages</th>
        <?PHP if (!empty($action) && $action=='edit') { ?><th width="25">&nbsp;</th><?PHP } ?>
    </tr>
    <?PHP if (!empty($possible_school_year)) { ?>
    <tr class="headers2">
        <?PHP foreach ($possible_school_year as $school_year):
            if (preg_match('/^([0-9]+)(re|e) – ([0-9]+)(re|e) ann(.+)$/u', $school_year, $matches)) {
                $school_year = $matches[1].'<sup>' .$matches[2].'</sup> &ndash; ' .$matches[3].'<sup>' .$matches[4].'</sup> ann' .$matches[5];
            }
        ?>
        <th width="<?PHP echo round(100/count($possible_school_year)); ?>%"><?PHP echo $school_year; ?></th>
        <?PHP endforeach; ?>
        <?PHP if (!empty($action) && $action=='edit') { ?><th width="25">&nbsp;</th><?PHP } ?>
    </tr>
    <?PHP } ?>
</thead>
<tbody id="course-body">
<?PHP
foreach ($objectiv->object_elements as $subtitle1) {
    if (!empty($subtitle1)&& !empty($subtitle1->subtitle)) {
?>
    <?PHP if (strtoupper($subtitle1->subtitle)!='GENERAL') { ?>
    <tr>
        <td class="titre titre1 titre1<?PHP echo $css_base_code; ?>" rowspan="1" colspan="<?PHP echo count($possible_school_year); ?>">
            <?PHP
            $url = $curriculum_per->buildUrl($subtitle1->url_part);
            echo '<a href="' .$url.'" target="_blank" class="per_link">' .
                  theme('image', array('path'=>drupal_get_path('module', 'archibald').'/images/info.png')).
                  '</a>';
            ?>
            <p><?PHP echo $subtitle1->subtitle; ?></p>
        </td>
        <?PHP if (!empty($action) && $action=='edit') { ?>
        <td class="action-edit">
             <a href="per|<?PHP echo $subtitle1->url_part; ?>" class="archibald_classification_remove" title="<?PHP echo t('Delete curriculum entry'); ?>"><img src="<?PHP echo base_path().drupal_get_path('module', 'archibald'); ?>/images/delete.png" border="0" /></a>
        </td>
        <?PHP } ?>
    </tr>
    <?PHP } ?>
    <?PHP
    $details = array();
    $details_independent = array();
    $trans = array_flip($possible_school_year);
    foreach ($subtitle1->details as $detail) {
        if (empty($detail->school_year)) {
            // for all
            $details_independent[] = $detail;
        } else {
            $details[ $trans[$detail->school_year] ][] = $detail;
        }
    }
    $max_i = 0;
    foreach ($details as $tmp) {
        $max_i = max($max_i, count($tmp));
    }
    ?>
    <?PHP
    if (!empty($details_independent)) {
        foreach ($details_independent as $detail) {
            $url = $curriculum_per->buildUrl($detail->url_part);
            ?>
    <tr>
            <td class="progression progression<?PHP echo $css_base_code; ?>" colspan="<?PHP echo count($possible_school_year); ?>">
                <div class="cell_content">
                    <?PHP
                    echo '<a href="' .$url.'" target="_blank" class="per_link">' .
                          theme('image', array('path'=>drupal_get_path('module', 'archibald').'/images/info.png')).
                          '</a>';
                    ?>
                    <p><?PHP echo nl2br(trim($detail->text)); ?></p>
                </div>
            </td>
            <?PHP if (!empty($action) && $action=='edit') { ?>
            <td class="action-edit">
                 <a href="per|<?PHP echo $detail->url_part; ?>" class="archibald_classification_remove" title="<?PHP echo t('Delete curriculum entry'); ?>"><img src="<?PHP echo base_path().drupal_get_path('module', 'archibald'); ?>/images/delete.png" border="0" /></a>
            </td>
            <?PHP } ?>
    </tr>
    <?PHP
        }
    }
    ?>

    <?PHP
    if ($max_i>0) {
        for ($i=0 ; $i<$max_i ; $i++) {
    ?>
    <tr>
        <?PHP
        $actions = array();
        for ($x=0 ; $x<count($possible_school_year) ; $x++) {
        ?>
            <?PHP if (empty($details[$x][$i])) { ?>
                <td class="progression progression<?PHP echo $css_base_code; ?>">&nbsp;</td>
                <?PHP
                if (!empty($action) && $action=='edit') {
                    $actions['delete'][$x] = theme('image', array('path'=>drupal_get_path('module', 'archibald').'/images/spacer.gif', 'width'=>'19'));
                }
                ?>
            <?PHP } else {
                $url = $curriculum_per->buildUrl($details[$x][$i]->url_part);
                ?>
                <td class="progression progression<?PHP echo $css_base_code; ?>">
                    <div class="cell_content">
                        <?PHP
                        echo '<a href="' .$url.'" target="_blank" class="per_link">' .
                              theme('image', array('path'=>drupal_get_path('module', 'archibald').'/images/info.png')).
                              '</a>';
                        ?>
                        <p><?PHP echo nl2br(trim($detail->text)); ?></p>
                    </div>
                </td>
                <?PHP
                if (!empty($action) && $action=='edit') {
                    $actions['delete'][$x] = '<a href="per|' .$details[$x][$i]->url_part.'" class="archibald_classification_remove" title="' . t('Delete curriculum entry') . '"><img src="' .base_path().drupal_get_path('module', 'archibald').'/images/delete.png" border="0" /></a>';
                }
                ?>
            <?PHP } ?>
        <?PHP } ?>
        <?PHP if (!empty($actions)) { ?>
        <td class="action-edit"><table border="0" class="action-edit-table">
            <?PHP foreach ($actions as $type=>$tmp) { ?>
            <tr><td><?PHP echo implode('</td><td>', $tmp); ?></td></tr>
            <?PHP } ?>
        </table></td>
        <?PHP } ?>
    </tr>
    <?PHP
        }
    }

    foreach ($subtitle1->childs as $subtitle2) {
        if (!empty($subtitle2) && !empty($subtitle2->subtitle)) { ?>
    <tr>
        <td class="titre titre2 titre2<?PHP echo $css_base_code; ?>" rowspan="1" colspan="<?PHP echo count($possible_school_year); ?>">
            <?PHP
            $url = $curriculum_per->buildUrl($subtitle2->url_part);
            echo '<a href="' .$url.'" target="_blank" class="per_link">' .
                  theme('image', array('path'=>drupal_get_path('module', 'archibald').'/images/info.png')).
                  '</a>';
            ?>
            <p><?PHP echo $subtitle2->subtitle; ?></p>
        </td>
        <?PHP if (!empty($action) && $action=='edit') { ?>
        <td class="action-edit">
             <a href="per|<?PHP echo $subtitle2->url_part; ?>" class="archibald_classification_remove" title="<?PHP echo t('Delete curriculum entry'); ?>"><img src="<?PHP echo base_path() . drupal_get_path('module', 'archibald'); ?>/images/delete.png" border="0" /></a>
        </td>
        <?PHP } ?>
    </tr>
    <?PHP
        $details = array();
        $details_independent = array();
        $trans = array_flip($possible_school_year);
        foreach ($subtitle2->details as $detail) {
            if (empty($detail->school_year)) {
                // for all
                $details_independent[] = $detail;
            } else {
                $details[ $trans[$detail->school_year] ][] = $detail;
            }
        }
        $max_i = 0;
        foreach ($details as $tmp) {
            $max_i = max($max_i, count($tmp));
        }
        ?>
        <?PHP
        if (!empty($details_independent)) {
            foreach ($details_independent as $detail) {
                $url = $curriculum_per->buildUrl($detail->url_part);
                ?>
        <tr>
            <td class="progression progression<?PHP echo $css_base_code; ?>" colspan="<?PHP echo count($possible_school_year); ?>">
                <div class="cell_content">
                    <?PHP
                    echo '<a href="' .$url.'" target="_blank" class="per_link">' .
                          theme('image', array('path'=>drupal_get_path('module', 'archibald').'/images/info.png')).
                          '</a>';
                    ?>
                    <p><?PHP echo nl2br(trim($detail->text)); ?></p>
                </div>
            </td>
            <?PHP if (!empty($action) && $action=='edit') { ?>
            <td class="action-edit">
                 <a href="per|<?PHP echo $detail->url_part; ?>" class="archibald_classification_remove" title="<?PHP echo t('Delete curriculum entry'); ?>"><img src="<?PHP echo base_path().drupal_get_path('module', 'archibald'); ?>/images/delete.png" border="0" /></a>
            </td>
            <?PHP } ?>
        </tr>
        <?PHP
            }
        }
        ?>


        <?PHP
        if ($max_i>0) {
            for ($i=0 ; $i<$max_i ; $i++) {
                $actions = array();
        ?>
        <tr>
            <?PHP for ($x=0 ; $x<count($possible_school_year) ; $x++) { ?>
                <?PHP if (empty($details[$x][$i])) { ?>
                    <td class="progression progression<?PHP echo $css_base_code; ?>">&nbsp;</td>
                    <?PHP
                    if (!empty($action) && $action=='edit') {
                        $actions['delete'][$x] = theme('image', array('path'=>drupal_get_path('module', 'archibald').'/images/spacer.gif', 'width'=>'19'));
                    }
                    ?>
                <?PHP } else {
                    $url = $curriculum_per->buildUrl($details[$x][$i]->url_part);
                    ?>
                    <td class="progression progression<?PHP echo $css_base_code; ?>">
                        <div class="cell_content">
                            <?PHP
                            echo '<a href="' .$url.'" target="_blank" class="per_link">' .
                                  theme('image', array('path'=>drupal_get_path('module', 'archibald').'/images/info.png')).
                                  '</a>';
                            ?>
                            <p><?PHP echo nl2br(trim($details[$x][$i]->text)); ?></p>
                        </div>
                    </td>
                    <?PHP
                    if (!empty($action) && $action=='edit') {
                        $actions['delete'][$x] = '<a href="per|' .$details[$x][$i]->url_part.'" class="archibald_classification_remove" title="' . t('Delete curriculum entry') . '"><img src="' .base_path().drupal_get_path('module', 'archibald').'/images/delete.png" border="0" /></a>';
                    }
                    ?>
                <?PHP } ?>
            <?PHP } ?>
            <?PHP if (!empty($actions)) { ?>
            <td class="action-edit"><table border="0" class="action-edit-table">
                <?PHP foreach ($actions as $type=>$tmp) { ?>
                <tr><td><?PHP echo implode('</td><td>', $tmp); ?></td></tr>
                <?PHP } ?>
            </table></td>
            <?PHP } ?>
        </tr>
        <?PHP
            }
        }
    }}
}}
?>
    </tbody>
</table>
