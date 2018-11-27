var margin = {
  top: 24,
  right: 50,
  bottom: 20,
  left: 50
};
var width = 390 - margin.left - margin.right;
var height = 390 - margin.top - margin.bottom;
var labelMargin = 10;

var scale = d3.scale.linear()
  .domain([0, 4])
  .range([0, 100])

d3.csv('cars.csv')
  .row(function(d) {
    d["Highway Miles Per Gallon"] = +d["Highway Miles Per Gallon"];
    d["Retail Price"] = +d["Retail Price"];
    d.Weight = +d.Weight;
    d["Engine Size (l)"] = d["Engine Size (l)"];
    d["Horsepower(HP)"] = +d["Horsepower(HP)"];
    d.Cyl = +d.Cyl;
    return d;
  })
  .get(function(error, rows) {
    var star = d3.starPlot()
      .width(width)
      //Because different attributes have different units, the values has to be
      //scaled, so that six attributes can be showed in one graphic.
      .accessors([
        function(d) {
          if (d["Highway Miles Per Gallon"]>0)
            return scale(d["Highway Miles Per Gallon"]/10);
          else return 0;  //Reset negative value as zero.
        },
        function(d) { return scale(d["Retail Price"]/20000); },
        function(d) { return scale(d.Weight/1500); },
        function(d) { return scale(d["Engine Size (l)"]/1.5); },
        function(d) { return scale(d["Horsepower(HP)"]/100); },
        function(d) { return scale(d.Cyl/2); },
      ])
      .labels([
        'Highway Miles/Gallon',
        'Retail Price',
        'Weight',
        'Engine Size (l)',
        'Horsepower',
        'Cyl',
      ])
      .title(function(d) { return d.Name; })
      .margin(margin)
      .labelMargin(labelMargin)
    rows.forEach(function(d, i) {
      d3.select('#star').append('svg')
        .attr('class', 'chart')
        .attr('width', width + margin.left + margin.right)
        .attr('height', width + margin.top + margin.bottom)
        .append('g')
        .datum(d)
        .call(star)
    });
  });
