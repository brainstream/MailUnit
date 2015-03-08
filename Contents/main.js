$(function () {
    if (document.location.hash) {
	$("#sidebar a[href=" + document.location.hash + "]").parent().addClass("active");
    }
    $("#sidebar .nav a").click(function () {
	$("#sidebar .nav li.active").removeClass("active");
	$(this).parent().addClass("active");
    });
})
