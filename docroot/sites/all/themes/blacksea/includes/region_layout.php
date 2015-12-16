<?php 

/*Css widths for top boxes*/

$topBlocks = 0;
if ($page['user1']) $topBlocks += 1;
if ($page['user2']) $topBlocks += 1;
if ($page['user3']) $topBlocks += 1;
if ($page['user4']) $topBlocks += 1;

switch ($topBlocks) {
	case 1:
		$topBlocks = "width100";
		break;
	case 2:
		$topBlocks = "width50";
		break;
	case 3:
		$topBlocks = "width33";
		break;
	case 4:
		$topBlocks = "width25";
		break;
	default:
		$topBlocks = "";
}

/*Css widths for bottom boxes*/

$bottomBlocks = 0;
if ($page['user5']) $bottomBlocks += 1;
if ($page['user6']) $bottomBlocks += 1;
if ($page['user7']) $bottomBlocks += 1;
if ($page['user8']) $bottomBlocks += 1;

switch ($bottomBlocks) {
	case 1:
		$bottomBlocks = "width100";
		break;
	case 2:
		$bottomBlocks = "width50";
		break;
	case 3:
		$bottomBlocks = "width33";
		break;
	case 4:
		$bottomBlocks = "width25";
		break;
	default:
		$bottomBlocks = "";
}

