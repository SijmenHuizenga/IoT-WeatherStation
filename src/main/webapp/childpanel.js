const Chart = ReactGoogleCharts.default.Chart;

class ChildPanel extends React.Component {

    constructor(props) {
        super(props);
        this.state = {brightnesses: [], humidities: [], pressures: [], temperatures: []};
    }

    componentDidMount() {
        let _this = this;
        this.serverRequest =
            axios.get("/child/"+this.props.id)
                .then(function(result) {
                    _this.setState({
                        brightnesses: result.data.brightnesses.map(function(e){ return [new Date(e.moment*1000), e.brightness]}),
                        humidities: result.data.humidities.map(function(e){ return [new Date(e.moment*1000), e.humidity]}),
                        pressures: result.data.pressures.map(function(e){ return [new Date(e.moment*1000), e.pressure]}),
                        temperatures: result.data.temperatures.map(function(e){ return [new Date(e.moment*1000), e.temperature]}),
                    });
                });
    }

    componentWillUnmount() {
        this.serverRequest.abort();
    }

    render() {
        return <div className="panel panel-default">
            <div className="panel-heading">
                <h3 className="panel-title" style={{float: "left"}}>{this.props.name} ({this.props.id})</h3>
                <span className="text-muted" style={{float: "right"}}>{this.props.ip}</span>
                <div style={{clear: "both"}} />
            </div>
            <div className="panel-body">
                <div className="row">
                    <MeasurementChart title="Brightness" data={this.state.brightnesses} id={this.props.id}/>
                    <MeasurementChart title="Temperature" data={this.state.temperatures} id={this.props.id}/>
                    <MeasurementChart title="Humidity" data={this.state.humidities} id={this.props.id}/>
                    <MeasurementChart title="Pressure" data={this.state.pressures} id={this.props.id}/>
                </div>
                <div className="row">
                    <div className="col-xs-12">
                        <ReactBootstrapSlider
                            value={[15, 25]}
                            slideStop={this.changeValue}
                            step={0.01}
                            max={100}
                            range={true}
                            min={0}/>
                    </div>
                </div>
            </div>
        </div>;
    }

    changeValue(newVal){
        console.log(newVal);
    }
}

class MeasurementChart extends React.Component {

    constructor(props) {
        super(props);
    }

    render() {
        return <div className="col-lg-3 col-md-3 col-sm-6 col-xs-12">
            <h4>{this.props.title}</h4>
            {this.props.data.length > 0 ?
                < Chart
                    data={[
                    ['Moment', 'Value'],
                    ...this.props.data
                    ]}
                    width="100%"
                    options={{
                        curveType: 'function',
                        legend: {position: 'none'},
                        chartArea: {'width': '100%', height: '90%', top: 0, left: 0},
                        explorer: {
                            actions:  ['dragToZoom', 'rightClickToReset'],
                            axis: 'horizontal',
                            keepInBounds: true,
                            maxZoomIn: 0.01,
                            maxZoomOut: 1
                        }
                    }}
                    graph_id={this.props.id + this.props.title}
                    loadCharts

                    /> : <span>No data available</span>
            }
            <br/>
        </div>
    }
}