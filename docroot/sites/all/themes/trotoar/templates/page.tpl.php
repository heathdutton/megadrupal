<?php
?>

    <div<?php print $trp_attributes; ?>>
		
		<?php if($atas): ?>
		<header<?php print $trh_attributes; ?>>
			<?php print $atas; ?>
		</header>
		<?php endif; ?>
		
		<?php if($tengah): ?>
        <section<?php print $trs_attributes; ?>>
            <?php print $tengah; ?>
		</section>
        <?php endif; ?>
		
		<?php if($bawah): ?>
		<footer<?php print $trf_attributes; ?>>
			<?php print $bawah; ?>
        </footer>
		<?php endif; ?>
		
	</div>
