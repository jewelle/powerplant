<?php
/*************************************************************************
remove visitor from audience.db when they click the join button
==========================================================================
adapted from www.thomas.vanhoutte.be/miniblog/visitor-counter/
*************************************************************************/

$adfile = "audience.db"; // path to file storing number of visitors

if(!file_exists($adfile)) {
    die("Error: Data file " . $adfile . " NOT FOUND!");
}
if(!is_writable($adfile)) {
    die("Error: Data file " . $adfile . " is NOT writable! Please CHMOD it to 666!");
}

function RemoveVisitor() {
    global $adfile;
    $cur_ip = getIP();
    $dbary_new = array();
 
    // unpack array and re-add everyone who is not the current user
    $dbary = unserialize(file_get_contents($adfile));
    if(is_array($dbary)) {
        while(list($user_ip, $user_time) = each($dbary)) {
            if($user_ip != $cur_ip) { // check whether the ip is the same
                $dbary_new[$user_ip] = $user_time; // adding them into the new array if not the same
            }
        }
    }

    $fp = fopen($adfile, "w");
    fputs($fp, serialize($dbary_new));
    fclose($fp);
 
 // this will become visitornum
    $out = sprintf("%03d", count($dbary_new)); // format the result to display 3 digits with leading 0's
    return $out;
}

function getIP() {
    if(isset($_SERVER['HTTP_X_FORWARDED_FOR'])) $ip = $_SERVER['HTTP_X_FORWARDED_FOR'];
    elseif(isset($_SERVER['REMOTE_ADDR'])) $ip = $_SERVER['REMOTE_ADDR'];
    else $ip = "0";
    return $ip;
}

$audiencenum = RemoveVisitor();

$fp = fopen('visitornum.txt', 'w');
fwrite($fp, $audiencenum);
fclose($fp);
?>
