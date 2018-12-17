var svg = d3.select("body").append("svg")
    .attr("width", 500)
    .attr("height", 500)
    .append("g")
        .attr("transform", "translate(50, 50)");

    svg.append("rect")
        .attr("fill", "none")
        .attr("pointer-events", "all")
        .attr("width", 500)
        .attr("height", 500)
        .call(d3.zoom()
            .scaleExtent([1, 8])
            .on("zoom", zoom));

    function zoom() {
      circle.attr("transform", transform(d3.event.transform));
    }

    function transform(t) {
      return function(d) {
        return "translate(" + t.apply(d) + ")";
      };
}
var tree = d3.tree()
    .size([400, 400]);

d3.json("web-vis-tools.json", function (data) {

    var nodes = tree.nodes(data);
    var links = tree.links(nodes);



    var diagonal = d3.svg.diagonal()
        .projection(function(d) { return [d.y, d.x]; });

    canvas.selectAll(".link")
        .data(links)
        .enter()
        .append("path")
        .attr("class", "link")
        .attr("fill", "none")
        .attr("stroke", "#ADADAD")
        .attr("d", diagonal);

    var node = canvas.selectAll(".node")
        .data(nodes)
        .enter()
        .append("g")
            .attr("class", "node")
            .attr("transform", function (d) { return "translate(" + d.y + "," + d.x + ")"; })

    node.append("circle")
        .attr("r", 3)
        .attr("fill", "steelblue");

    node.append("text")
        .text(function (d) {return d.name; })
        })
