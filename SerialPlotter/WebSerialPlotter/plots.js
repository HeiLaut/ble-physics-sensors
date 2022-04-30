function drawDia(){
  PLOT = document.getElementById('plot');

  var trace1 ={
    x: x,
    y: y,
    type: 'scatter'
  }
  var data = [trace1];
  var layout = {
    xaxis:{
      title: labelX,
      titlefont:
      {
        size:18
      },
      gridcolor: '#bdbdbd',

      tickfont:{
        size:18,
      }
    },
    yaxis:{
      title: labelY,
      titlefont:
      {
        size:18
      },
      gridcolor: '#bdbdbd',

      tickfont:{
        size:18,
      }
    }
    }

	Plotly.newPlot( PLOT, data,layout);
}
