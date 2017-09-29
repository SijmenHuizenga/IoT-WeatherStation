package nl.han.weatherboys.jorgapi;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Calendar;

abstract class JorgApiRequest {

    private static final SimpleDateFormat TIMESTAMP_FORMAT = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss.X");
    private static final SimpleDateFormat TIMESTAMP_FORMAT_OUT = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss.0000000X");
    private static final SimpleDateFormat DATE_FORMAT = new SimpleDateFormat("EEE, dd MMM yyyy HH:mm:ss z");

    /**
     * If the response contains some kind of error, than a error string will be constructed and thrown as a IOException.
     */
    void handleErrorResponse(int statuscode, JSONObject body) throws IOException {
        try {
            if(statuscode != 200) {
                String error = statuscode + ":";
                if(body.has("error"))
                    error += " " + body.getString("error");
                if(body.has("error_description"))
                    error += " " + body.getString("error_description");
                throw new IOException(error);
            }
        } catch (JSONException e) {
            throw new IOException(e);
        }
    }

    /**
     * Given a timestamp string in a response from the Jorg Api returns a a unix second timestamp. Or null if the format is invalid.
     * Example: 2017-09-19T09:29:44.1648619Z
     */
    Long makeTimestamp(String txt){
        try{
            Calendar c = Calendar.getInstance();
            //substring to remove the 7 digit fractal seconds and add back the last timezone char
            c.setTime(TIMESTAMP_FORMAT.parse(txt.substring(0, 20)+txt.charAt(27)));
            return c.getTimeInMillis()/1000L;
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }

    /**
     * Given a datetime string in a response from the Jorg Api returns a unix second timestamp. Or null if the format is invalid.
     * Example: Wed, 20 Sep 2017 12:26:47 GMT
     */
    Long makeDateTime(String txt){
        try{
            Calendar c = Calendar.getInstance();
            c.setTime(DATE_FORMAT.parse(txt));
            return c.getTimeInMillis()/1000L;
        } catch (ParseException e) {
            e.printStackTrace();
            return null;
        }
    }

    /**
     * Given a unix second timestamp return a timestamp in the following format:
     *    2017-09-20T15:53:47.0000000+02:00
     */
    String makeStringStamp(long timestamp){
        Calendar c = Calendar.getInstance();
        c.setTimeInMillis(timestamp*1000L);
        return TIMESTAMP_FORMAT_OUT.format(c.getTime());
    }

}
