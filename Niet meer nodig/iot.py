from oauthlib.oauth2 import BackendApplicationClient
from requests_oauthlib import OAuth2Session

from os import access
import iot_api_client as iot
from iot_api_client.rest import ApiException
from iot_api_client.configuration import Configuration

# Get your token 

oauth_client = BackendApplicationClient(client_id="18Nsf79ba4skDj3RgQjDqrHeYuFig0e3")
token_url = "https://api2.arduino.cc/iot/v1/clients/token"

oauth = OAuth2Session(client=oauth_client)
token = oauth.fetch_token(
    token_url=token_url,
    client_id="18Nsf79ba4skDj3RgQjDqrHeYuFig0e3",
    client_secret="XEiebYCu0STLGz9rqc1jfrSYGQx1JeoA0qXCdSVJHFTkEgBcofFizKpzKZoRfv2v",
    include_client_id=True,
    audience="https://api2.arduino.cc/iot",
)

# store access token in access_token variable
access_token = token.get("access_token")