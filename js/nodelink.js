//Working example from: https://stackoverflow.com/questions/14666543/d3-js-semantic-zoom-and-pan-example-not-working

var m = [20, 20, 20, 20],
        w = 1500 - m[1] - m[3],
        h = 600 - m[0] - m[2],
        i = 0,
        r = 800,
        x = d3.scale.linear().domain([0, w]).range([0, w]),
        y = d3.scale.linear().domain([0, h]).range([0, h]),
        root;
        zoom_threshold_text = 1;
d3.json("web-vis-tools.json", function(error, treedata) {

    var vis = d3.select("#tree").append("svg:svg")
                  .attr("viewBox", "0 0 600 600")
                  .attr("width", w + m[1] + m[3])
                  .attr("height", h + m[0] + m[2])
                  .append("svg:g")
                  .attr("transform", "translate(" + m[3] + "," + m[0] + ")")
                  .call(d3.behavior.zoom().x(x).y(y).scaleExtent([1,8]).on("zoom",zoom));

    var zoom = d3.behavior.zoom().x(x).y(y).scaleExtent([1,8]).on("zoom",zoom);

    vis.append("rect")
        .attr("class", "overlay")
        .attr("width", w + m[1] + m[3])
        .attr("height", h + m[0] + m[2])
        .attr("opacity", 0);

    var tree = d3.layout.tree()
        .size([h, w]);

    var diagonal = d3.svg.diagonal()
        .projection(function(d) { return [d.y, d.x]; });

    root = treedata;
    root.x0 = h / 2;
    root.y0 = 0;

    function toggleAll(d) {
      if (d.children) {
                  d.children.forEach(toggleAll);
        toggle(d);
      }
    };
    console.log(root)

    root.children.forEach(toggleAll);

    update(root);

    function update(source) {
      var duration = d3.event && d3.event.altKey ? 5000: 500;

      var nodes = tree.nodes(root).reverse();

      nodes.forEach(function (d) { d.y = d.depth * 180; });

      var node = vis.selectAll("g.node")
          .data(nodes, function(d) { return d.id || (d.id = ++i); });

      var nodeEnter = node.enter().append("svg:g")
          .attr("class", "node")
          .attr("transform", function(d) {return "translate(" + source.y0 + "," + source.x0 + ")"; })
          .on("click", function(d) {
              toggle(d);
              update(d);
              if (d['info']) {
                  playvid(d['info']);
              }
          });

      nodeEnter.append("svg:circle")
          .attr("r", 1e-6)
          .style("fill", function(d) { return d._children ? "lightsteelblue" : "#fff"; });

      nodeEnter.append("svg:text")
          .style("opacity", function(d) {if (zoom.scale() < zoom_threshold_text) { return 0} else {return 100};}) // Semantic Zooming with thresholds doesnt work sadly.
          .attr("x", function(d) { return d.children || d._children ? -10 : 10; })
          .attr("y", function(d) { return d.children || d._children ? -4 : 4; })
          .attr("dy", ".35em")
          .attr("text-anchor", function(d) { return d.children || d._children ? "end" : "start"; })
          .text(function(d) { return d.name; })
          .style("fill-opacity", 1e-6);

      var nodeUpdate = node.transition()
          .duration(duration)
          .attr("transform", function(d) { return "translate(" + d.y + "," + d.x + ")"; });

      nodeUpdate.select("circle")
          .attr("r", 6)
          .style("fill", function(d) { return d._children ? "lightsteelblue" : "#fff"; });

      nodeUpdate.select("text")
          .style("fill-opacity", 1);

      var nodeExit = node.exit().transition()
          .duration(duration)
          .attr("transform", function(d) { return "translate(" + source.y + "," + source.x + ")"; })
          .remove();

      nodeExit.select("circle")
          .attr("r", 1e-6);
      nodeExit.select("text")
          .style("fill-opacity", 1e-6);

      var link = vis.selectAll("path.link")
          .data(tree.links(nodes), function(d) { return d.target.id; });

      link.enter().insert("svg:path", "g")
          .attr("class", "link")
          .attr("d", function(d) {
            var o = {x: source.x0, y: source.y0};
            return diagonal({source: o, target: o});
          })
          .transition()
            .duration(duration)
            .attr("d", diagonal);

        link.style("stroke", function(d) {
            if (d.free = true) return "green";
            else return "red";});

      link.transition()
          .duration(duration)
          .attr("d", diagonal);

      link.exit().transition()
          .duration(duration)
          .attr("d", function(d) {
            var o = {x: source.x, y: source.y};
            return diagonal({source: o, target: o});
          })
          .remove();

      nodes.forEach(function(d) {
        d.x0 = d.x;
        d.y0 = d.y;
      });
    }
    function toggle(d) {
      if (d.children) {
        d._children = d.children;
        d.children = null;
      }
      else {
        d.children = d._children;
        d._children = null;
      }
    }

    function zoom(d) {
        var nodes = vis.selectAll("g.node");
        nodes.attr("transform", transform);

        var link = vis.selectAll("path.link");
        link.attr("d", translate);

         }

    function transform(d) {
        return "translate(" + x(d.y) + "," + y(d.x) + ")";
    }

    function translate(d) {
        var sourceX = x(d.target.parent.y);
        var sourceY = y(d.target.parent.x);
        var targetX = x(d.target.y);
        var targetY = (sourceX + targetX)/2;
        var linkTargetY = y(d.target.x0);
        var result = "M"+sourceX+","+sourceY+" C"+targetX+","+sourceY+" "+targetY+","+y(d.target.x0)+" "+targetX+","+linkTargetY+"";

       return result;
    }
    });