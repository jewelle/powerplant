joined = false;
var queuenum;
var accesstype;
var isMobile;

$(document).ready(function(){
    
    // set the css according to whether desktop or mobile (defined in index.html script)
    setSize();
    
    // determine whether visitor is using phone or desktop
    var isMobile = /iPhone|iPad|iPod|Android/i.test(navigator.userAgent);
    
    // when page loads, get visitor IP and time, store in DB and update DB + .txt file re: expired visitors.
    // .txt file is read by Arduino to change lights.
    // read results from visitors.php (current # of visitors) into html
    
    $.ajax({
        url: "visitors.php",
        type: "get",
        success: function (data){
            queuenum = data;
            checkButton();
            }
    });
    
    
    // change css if window is resized
	
    $(window).resize(function(){
        setSize();
    });

    // run  visitors.php again and read results into html visitornum
    // set timeout again for another 3 seconds
    
    function refreshActivity() {
        $.ajax({ // send status of joined somehow
            url: "visitors.php",
            type: "get",
            success: function (data){
                // put function here that changes the button values
                queuenum = data;
                checkButton();
                },
            complete: function (data){
                setTimeout(refreshActivity, 3000);
            }
        });
    }


    // refresh activity every 3 seconds.
    
    var runless  = setTimeout(refreshActivity, 3000);
    
    
    // changes button based on number in queue
    
    function checkButton() {
        if (queuenum >= 4){
                // disable and turn red
                $('#btn_joinleavewait').attr("disabled", true);
                $('#btn_joinleavewait').css('background-color', '#f04e4f');
                $('#btn_joinleavewait').html("WAIT");
                $('#textbox').html("Please wait until someone leaves the installation. There are more than three people online, meeting capacity.");
        }
        if (queuenum < 4){
            $('#btn_joinleavewait').attr("disabled",false);
            $('#btn_joinleavewait').css('background-color', 'white');
            if (joined == false){
                $('#btn_joinleavewait').html("JOIN"); 
                if (isMobile){
                    checkPlayingJoin();
                }
                else{
                    $('#textbox').html("Click JOIN to view the power you are consuming by visiting this site.");
                }
            }
            if (joined == true){
                $('#btn_joinleavewait').html("LEAVE"); 
                if (isMobile){
                    checkPlayingLeave();
                }
                else{
                    $('#textbox').html("Please be patient, this livestream has a 20 sec - 1 min delay.");
                }
            }
        }
    }
    
    // button click to add or remove IP from audience.db using join.php or leave.php

    $('#btn_joinleavewait').click(function(){
        if (joined == false){
            $.ajax({
                url: "join.php",
                type: "get",
            });
            joined = true;
            $('#btn_joinleavewait').html("LEAVE"); 
            if (isMobile){
                checkPlayingLeave();
            }
            else{
                $('#textbox').html("Please be patient, this livestream has a 20 sec - 1 min delay.");
            }
        }
        else if (joined == true){
            $.ajax({
                url: "leave.php",
                type: "get",
            });
            joined = false;
            $('#btn_joinleavewait').html("JOIN"); 
            if (isMobile){
                checkPlayingJoin();
            }
            else{
                $('#textbox').html("Click JOIN to view the power you are consuming by visiting this site.");
            }
        }
    });  
    
    //when the user leaves the page, run the leavers.php file to remove their IP from the list. doesn't work well in mobile browsers.
    
    $(window).on('beforeunload', function() {
	    navigator.sendBeacon('leavers.php');
    });
    window.addEventListener("unload", close, false);
    function close() {
        navigator.sendBeacon("leavers.php");
    }
});
