var margin = {
  top: 36,
  right: 50,
  bottom: 20,
  left: 50
};
var width = 240 - margin.left - margin.right;
var height = 240 - margin.top - margin.bottom;
var labelMargin = 8;

var scale = d3.scale.linear()
  .domain([0, 4])
  .range([0, 100])

d3.csv('cars.csv')
  .row(function(d) {
    d["Highway Miles Per Gallon"] = +d["Highway Miles Per Gallon"];
    d["Retail Price"] = +d["Retail Price"];
    d["Dealer Cost"] = +d["Dealer Cost"];
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
        function(d) { return scale(d["Dealer Cost"]/20000); },
        function(d) { return scale(d["Engine Size (l)"]/1.5); },
        function(d) { return scale(d["Horsepower(HP)"]/100); },
        function(d) { return scale(d.Cyl/2); },
      ])
      .labels([
        'Highway Miles/Gallon',
        'Retail Price',
        'Dealer Cost',
        'Engine Size (l)',
        'Horsepower',
        'Cyl',
      ])
      .title(function(d) { return d.Name; })
      .margin(margin)
      .labelMargin(labelMargin)

    rows.forEach(function(d, i) {
      star.includeLabels(i % 4 === 0 ? true : false);

      d3.select('#target').append('svg')
        .attr('class', 'chart')
        .attr('width', width + margin.left + margin.right)
        .attr('height', width + margin.top + margin.bottom)
        .append('g')
        .datum(d)
        .call(star)
    });
  });
