var margin = {top: 20, right: 30, bottom: 30, left: 60},
    width = 1000 - margin.left - margin.right,
    height = 500 - margin.top - margin.bottom;

    var xValue = function(d) { return d["Retail Price"];},
        xScale = d3.scale.linear().range([0, width]),
        xMap = function(d) { return xScale(xValue(d));},
        xAxis = d3.svg.axis().scale(xScale).orient("bottom");

    var yValue = function(d) { return d.Weight;},
        yScale = d3.scale.linear().range([height, 0]),
        yMap = function(d) { return yScale(yValue(d));},
        yAxis = d3.svg.axis().scale(yScale).orient("left");

    var cValue = function(d) { return d.Type;},
        color = d3.scale.category10();

    var rValue = function(d) { return d["Highway Miles Per Gallon"];}

    var svg = d3.select("#ScaPlot").append("svg")
        .attr("width", width + margin.left + margin.right)
        .attr("height", height + margin.top + margin.bottom)
      .append("g")
        .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

    var div = d3.select("#ScaPlot")
        .append("div")
        .attr("class", "tooltip")
        .style("opacity", 0);

    d3.csv("cars.csv", function(error, data) {
      data.forEach(function(d) {
        d["Retail Price"] = +d["Retail Price"];
        d.Weight = +d.Weight;
      });
      xScale.domain([10000, 130000]);
      yScale.domain([d3.min(data, yValue)-10, d3.max(data, yValue)+10]);

      svg.append("g")
          .attr("class", "x axis")
          .attr("transform", "translate(0," + height + ")")
          .call(xAxis)
        .append("text")
          .attr("class", "label")
          .attr("x", width)
          .attr("y", -6)
          .style("text-anchor", "end")
          .text("Retail Price");

      svg.append("g")
          .attr("class", "y axis")
          .call(yAxis)
        .append("text")
          .attr("class", "label")
          .attr("transform", "rotate(-90)")
          .attr("y", 6)
          .attr("dy", ".71em")
          .style("text-anchor", "end")
          .text("Weight in Pounds");

        svg.selectAll(".dot")
          .data(data)
          .enter()
          .append("circle")
          .attr("class", "dot")
          .attr("r", function(d) {
              return (rValue(d)/4.5);
          })
          .attr("cx", xMap)
          .attr("cy", yMap)
          .attr("opacity", 0.5)
          .style("fill", function(d) { return color(cValue(d));})
          .on("mouseover", function(d) {
              div.transition()
                   .duration(200)
                   .style("opacity", .9);
              div.html(d["Name"] + "<br/> (" + xValue(d)
    	        + ", " + yValue(d) + ")")
                   .style("left", (d3.event.pageX + 10) + "px")
                   .style("top", (d3.event.pageY - 15) + "px");
              d3.select(this).transition()
                   .duration(200)
                   .style("opacity", 1.0);
          })
          .on("mouseout", function(d) {
              div.transition()
                   .duration(500)
                   .style("opacity", .0);
              d3.select(this).transition()
                   .duration(500)
                   .style("opacity", 0.5);
          });

      var legend = svg.selectAll(".legend")
          .data(color.domain())
          .enter()
          .append("g")
          .attr("class", "legend")
          .attr("transform", function(d, i) { return "translate(0," + i * 35 + ")"; });

      legend.append("rect")
          .attr("x", width - 15)
          .attr("y", 110)
          .attr("width", 15)
          .attr("height", 15)
           .attr("opacity", 0.7)
          .style("fill", color);

      legend.append("text")
          .attr("x", width - 24)
          .attr("y", 119)
          .attr("dy", ".35em")
          .style("text-anchor", "end")
          .text(function(d) { return d;})
    });